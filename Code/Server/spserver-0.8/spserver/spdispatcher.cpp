/*
 * Copyright 2007 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <syslog.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include "spdispatcher.hpp"

#include "speventcb.hpp"
#include "sphandler.hpp"
#include "spsession.hpp"
#include "spexecutor.hpp"
#include "sputils.hpp"
#include "spiochannel.hpp"
#include "spioutils.hpp"

#include "config.h"
#include "event_msgqueue.h"

SP_Dispatcher :: SP_Dispatcher( SP_CompletionHandler * completionHandler, int maxThreads )
{
	/* Don't die with SIGPIPE on remote read shutdown. That's dumb. */
	signal( SIGPIPE, SIG_IGN );

	mIsShutdown = 0;
	mIsRunning = 0;

	mEventArg = new SP_EventArg( 600 );

	mMaxThreads = maxThreads > 0 ? maxThreads : 64;

	mCompletionHandler = completionHandler;

	mPushQueue = msgqueue_new( mEventArg->getEventBase(), 0, onPush, mEventArg );
}

SP_Dispatcher :: ~SP_Dispatcher()
{
	if( 0 == mIsRunning ) sleep( 1 );

	shutdown();

	for( ; mIsRunning; ) sleep( 1 );

	//msgqueue_destroy( (struct event_msgqueue*)mPushQueue );

	delete mEventArg;
	mEventArg = NULL;
}

void SP_Dispatcher :: setTimeout( int timeout )
{
	mEventArg->setTimeout( timeout );
}

void SP_Dispatcher :: shutdown()
{
	mIsShutdown = 1;
}

int SP_Dispatcher :: isRunning()
{
	return mIsRunning;
}

int SP_Dispatcher :: getSessionCount()
{
	return mEventArg->getSessionManager()->getCount();
}

int SP_Dispatcher :: getReqQueueLength()
{
	return mEventArg->getInputResultQueue()->getLength();
}

int SP_Dispatcher :: dispatch()
{
	int ret = -1;

	pthread_attr_t attr;
	pthread_attr_init( &attr );
	assert( pthread_attr_setstacksize( &attr, 1024 * 1024 ) == 0 );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

	pthread_t thread = 0;
	ret = pthread_create( &thread, &attr, reinterpret_cast<void*(*)(void*)>(eventLoop), this );
	pthread_attr_destroy( &attr );
	if( 0 == ret ) {
		syslog( LOG_NOTICE, "Thread #%ld has been created for dispatcher", thread );
	} else {
		mIsRunning = 0;
		syslog( LOG_WARNING, "Unable to create a thread for dispatcher, %s",
			strerror( errno ) ) ;
	}

	return ret;
}

void * SP_Dispatcher :: eventLoop( void * arg )
{
	SP_Dispatcher * dispatcher = (SP_Dispatcher*)arg;

	dispatcher->mIsRunning = 1;

	dispatcher->start();

	dispatcher->mIsRunning = 0;

	return NULL;
}

void SP_Dispatcher :: outputCompleted( void * arg )
{
	SP_CompletionHandler * handler = ( SP_CompletionHandler * ) ((void**)arg)[0];
	SP_Message * msg = ( SP_Message * ) ((void**)arg)[ 1 ];

	handler->completionMessage( msg );

	free( arg );
}

int SP_Dispatcher :: start()
{
	SP_Executor workerExecutor( mMaxThreads, "work" );
	SP_Executor actExecutor( 1, "act" );

	/* Start the event loop. */
	while( 0 == mIsShutdown ) {
		event_base_loop( mEventArg->getEventBase(), EVLOOP_ONCE );

		for( ; NULL != mEventArg->getInputResultQueue()->top(); ) {
			SP_Task * task = (SP_Task*)mEventArg->getInputResultQueue()->pop();
			workerExecutor.execute( task );
		}

		for( ; NULL != mEventArg->getOutputResultQueue()->top(); ) {
			SP_Message * msg = (SP_Message*)mEventArg->getOutputResultQueue()->pop();

			void ** arg = ( void** )malloc( sizeof( void * ) * 2 );
			arg[ 0 ] = (void*)mCompletionHandler;
			arg[ 1 ] = (void*)msg;

			actExecutor.execute( outputCompleted, arg );
		}
	}

	syslog( LOG_NOTICE, "Dispatcher is shutdown." );

	return 0;
}

typedef struct tagSP_PushArg {
	int mType;      // 0 : fd, 1 : timer

	// for push fd
	int mFd;
	SP_Handler * mHandler;
	SP_IOChannel * mIOChannel;
	int mNeedStart;

	// for push timer
	struct timeval mTimeout;
	struct event mTimerEvent;
	SP_TimerHandler * mTimerHandler;
	SP_EventArg * mEventArg;
	void * mPushQueue;
} SP_PushArg_t;

void SP_Dispatcher :: onPush( void * queueData, void * arg )
{
	SP_PushArg_t * pushArg = (SP_PushArg_t*)queueData;
	SP_EventArg * eventArg = (SP_EventArg*)arg;

	if( 0 == pushArg->mType ) {
		SP_Sid_t sid;
		sid.mKey = pushArg->mFd;
		eventArg->getSessionManager()->get( sid.mKey, &sid.mSeq );

		SP_Session * session = new SP_Session( sid );

		eventArg->getSessionManager()->put( sid.mKey, session, &sid.mSeq );

		session->setHandler( pushArg->mHandler );
		session->setIOChannel( pushArg->mIOChannel );
		session->setArg( eventArg );

		event_set( session->getReadEvent(), pushArg->mFd, EV_READ,
				SP_EventCallback::onRead, session );
		event_set( session->getWriteEvent(), pushArg->mFd, EV_WRITE,
				SP_EventCallback::onWrite, session );

		if( pushArg->mNeedStart ) {
			SP_EventHelper::doStart( session );
		} else {
			SP_EventCallback::addEvent( session, EV_WRITE, pushArg->mFd );
			SP_EventCallback::addEvent( session, EV_READ, pushArg->mFd );
		}

		free( pushArg );
	} else {
		event_set( &( pushArg->mTimerEvent ), -1, 0, onTimer, pushArg );
		event_base_set( eventArg->getEventBase(), &( pushArg->mTimerEvent ) );
		event_add( &( pushArg->mTimerEvent ), &( pushArg->mTimeout ) );
	}
}

int SP_Dispatcher :: push( int fd, SP_Handler * handler, int needStart )
{
	SP_IOChannel * ioChannel = new SP_DefaultIOChannel();
	return push( fd, handler, ioChannel, needStart );
}

int SP_Dispatcher :: push( int fd, SP_Handler * handler,
		SP_IOChannel * ioChannel, int needStart )
{
	uint16_t seq = 0;
	if( NULL != mEventArg->getSessionManager()->get( fd, &seq ) ) {
		return -1;
	}

	SP_PushArg_t * arg = (SP_PushArg_t*)malloc( sizeof( SP_PushArg_t ) );
	arg->mType = 0;
	arg->mFd = fd;
	arg->mHandler = handler;
	arg->mIOChannel = ioChannel;
	arg->mNeedStart = needStart;

	SP_IOUtils::setNonblock( fd );

	return msgqueue_push( (struct event_msgqueue*)mPushQueue, arg );
}

void SP_Dispatcher :: onTimer( int, short, void * arg )
{
	SP_PushArg_t * pushArg = (SP_PushArg_t*)arg;

	pushArg->mEventArg->getInputResultQueue()->push(
		new SP_SimpleTask( timer, pushArg, 1 ) );
}

void SP_Dispatcher :: timer( void * arg )
{
	SP_PushArg_t * pushArg = (SP_PushArg_t*)arg;
	SP_TimerHandler * handler = pushArg->mTimerHandler;
	SP_EventArg * eventArg = pushArg->mEventArg;

	SP_Sid_t sid;
	sid.mKey = SP_Sid_t::eTimerKey;
	sid.mSeq = SP_Sid_t::eTimerSeq;
	SP_Response * response = new SP_Response( sid );
	if( 0 == handler->handle( response, &( pushArg->mTimeout ) ) ) {
		msgqueue_push( (struct event_msgqueue*)pushArg->mPushQueue, arg );
	} else {
		delete pushArg->mTimerHandler;
		free( pushArg );
	}

	msgqueue_push( (struct event_msgqueue*)eventArg->getResponseQueue(), response );
}

int SP_Dispatcher :: push( const struct timeval * timeout, SP_TimerHandler * handler )
{
	SP_PushArg_t * arg = (SP_PushArg_t*)malloc( sizeof( SP_PushArg_t ) );

	arg->mType = 1;
	arg->mTimeout = *timeout;
	arg->mTimerHandler = handler;
	arg->mEventArg = mEventArg;
	arg->mPushQueue = mPushQueue;

	return msgqueue_push( (struct event_msgqueue*)mPushQueue, arg );
}


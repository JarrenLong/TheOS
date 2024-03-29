/*
 * Copyright 2007 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/tcp.h>

#include "spserver.hpp"
#include "speventcb.hpp"
#include "sphandler.hpp"
#include "spsession.hpp"
#include "spexecutor.hpp"
#include "sputils.hpp"
#include "spiochannel.hpp"
#include "spioutils.hpp"

#include "config.h"
#include "event_msgqueue.h"

SP_Server :: SP_Server( const char * bindIP, int port,
		SP_HandlerFactory * handlerFactory )
{
	snprintf( mBindIP, sizeof( mBindIP ), "%s", bindIP );
	mPort = port;
	mIsShutdown = 0;
	mIsRunning = 0;

	mHandlerFactory = handlerFactory;
	mIOChannelFactory = NULL;

	mTimeout = 600;
	mMaxThreads = 64;
	mReqQueueSize = 128;
	mMaxConnections = 256;
	mRefusedMsg = strdup( "System busy, try again later." );
}

SP_Server :: ~SP_Server()
{
	if( NULL != mHandlerFactory ) delete mHandlerFactory;
	mHandlerFactory = NULL;

	if( NULL != mIOChannelFactory ) delete mIOChannelFactory;
	mIOChannelFactory = NULL;

	if( NULL != mRefusedMsg ) free( mRefusedMsg );
	mRefusedMsg = NULL;
}

void SP_Server :: setIOChannelFactory( SP_IOChannelFactory * ioChannelFactory )
{
	mIOChannelFactory = ioChannelFactory;
}

void SP_Server :: setTimeout( int timeout )
{
	mTimeout = timeout > 0 ? timeout : mTimeout;
}

void SP_Server :: setMaxThreads( int maxThreads )
{
	mMaxThreads = maxThreads > 0 ? maxThreads : mMaxThreads;
}

void SP_Server :: setMaxConnections( int maxConnections )
{
	mMaxConnections = maxConnections > 0 ? maxConnections : mMaxConnections;
}

void SP_Server :: setReqQueueSize( int reqQueueSize, const char * refusedMsg )
{
	mReqQueueSize = reqQueueSize > 0 ? reqQueueSize : mReqQueueSize;

	if( NULL != mRefusedMsg ) free( mRefusedMsg );
	mRefusedMsg = strdup( refusedMsg );
}

void SP_Server :: shutdown()
{
	mIsShutdown = 1;
}

int SP_Server :: isRunning()
{
	return mIsRunning;
}

int SP_Server :: run()
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
		syslog( LOG_NOTICE, "Thread #%ld has been created to listen on port [%d]", thread, mPort );
	} else {
		mIsRunning = 0;
		syslog( LOG_WARNING, "Unable to create a thread for TCP server on port [%d], %s",
			mPort, strerror( errno ) ) ;
	}

	return ret;
}

void SP_Server :: runForever()
{
	eventLoop( this );
}

void * SP_Server :: eventLoop( void * arg )
{
	SP_Server * server = (SP_Server*)arg;

	server->mIsRunning = 1;

	server->start();

	server->mIsRunning = 0;

	return NULL;
}

void SP_Server :: sigHandler( int, short, void * arg )
{
	SP_Server * server = (SP_Server*)arg;
	server->shutdown();
}

void SP_Server :: outputCompleted( void * arg )
{
	SP_CompletionHandler * handler = ( SP_CompletionHandler * ) ((void**)arg)[0];
	SP_Message * msg = ( SP_Message * ) ((void**)arg)[ 1 ];

	handler->completionMessage( msg );

	free( arg );
}

int SP_Server :: start()
{
	/* Don't die with SIGPIPE on remote read shutdown. That's dumb. */
	signal( SIGPIPE, SIG_IGN );

	int ret = 0;
	int listenFD = -1;

	ret = SP_IOUtils::tcpListen( mBindIP, mPort, &listenFD, 0 );

	if( 0 == ret ) {

		SP_EventArg eventArg( mTimeout );

		// Clean close on SIGINT or SIGTERM.
		struct event evSigInt, evSigTerm;
		signal_set( &evSigInt, SIGINT,  sigHandler, this );
		event_base_set( eventArg.getEventBase(), &evSigInt );
		signal_add( &evSigInt, NULL);
		signal_set( &evSigTerm, SIGTERM, sigHandler, this );
		event_base_set( eventArg.getEventBase(), &evSigTerm );
		signal_add( &evSigTerm, NULL);

		SP_AcceptArg_t acceptArg;
		memset( &acceptArg, 0, sizeof( SP_AcceptArg_t ) );

		if( NULL == mIOChannelFactory ) {
			mIOChannelFactory = new SP_DefaultIOChannelFactory();
		}
		acceptArg.mEventArg = &eventArg;
		acceptArg.mHandlerFactory = mHandlerFactory;
		acceptArg.mIOChannelFactory = mIOChannelFactory;
		acceptArg.mReqQueueSize = mReqQueueSize;
		acceptArg.mMaxConnections = mMaxConnections;
		acceptArg.mRefusedMsg = mRefusedMsg;

		struct event evAccept;
		event_set( &evAccept, listenFD, EV_READ|EV_PERSIST,
				SP_EventCallback::onAccept, &acceptArg );
		event_base_set( eventArg.getEventBase(), &evAccept );
		event_add( &evAccept, NULL );

		SP_Executor workerExecutor( mMaxThreads, "work" );
		SP_Executor actExecutor( 1, "act" );
		SP_CompletionHandler * completionHandler = mHandlerFactory->createCompletionHandler();

		/* Start the event loop. */
		while( 0 == mIsShutdown ) {
			event_base_loop( eventArg.getEventBase(), EVLOOP_ONCE );

			for( ; NULL != eventArg.getInputResultQueue()->top(); ) {
				SP_Task * task = (SP_Task*)eventArg.getInputResultQueue()->pop();
				workerExecutor.execute( task );
			}

			for( ; NULL != eventArg.getOutputResultQueue()->top(); ) {
				SP_Message * msg = (SP_Message*)eventArg.getOutputResultQueue()->pop();

				void ** arg = ( void** )malloc( sizeof( void * ) * 2 );
				arg[ 0 ] = (void*)completionHandler;
				arg[ 1 ] = (void*)msg;

				actExecutor.execute( outputCompleted, arg );
			}
		}

		delete completionHandler;

		syslog( LOG_NOTICE, "Server is shutdown." );

		event_del( &evAccept );

		signal_del( &evSigTerm );
		signal_del( &evSigInt );

		close( listenFD );
	}

	return ret;
}


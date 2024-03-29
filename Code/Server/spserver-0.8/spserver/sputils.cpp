/*
 * Copyright 2007 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "sputils.hpp"

const int SP_ArrayList::LAST_INDEX = -1;

SP_ArrayList :: SP_ArrayList( int initCount )
{
	mMaxCount = initCount <= 0 ? 2 : initCount;
	mCount = 0;
	mFirst = (void**)malloc( sizeof( void * ) * mMaxCount );
}

SP_ArrayList :: ~SP_ArrayList()
{
	free( mFirst );
	mFirst = NULL;
}

int SP_ArrayList :: getCount() const
{
	return mCount;
}

int SP_ArrayList :: append( void * value )
{
	if( NULL == value ) return -1;

	if( mCount >= mMaxCount ) {
		mMaxCount = ( mMaxCount * 3 ) / 2 + 1;
		mFirst = (void**)realloc( mFirst, sizeof( void * ) * mMaxCount );
		assert( NULL != mFirst );
		memset( mFirst + mCount, 0, ( mMaxCount - mCount ) * sizeof( void * ) );
	}

	mFirst[ mCount++ ] = value;

	return 0;
}

void * SP_ArrayList :: takeItem( int index )
{
	void * ret = NULL;

	if( LAST_INDEX == index ) index = mCount -1;
	if( index < 0 || index >= mCount ) return ret;

	ret = mFirst[ index ];

	mCount--;

	if( ( index + 1 ) < mMaxCount ) {
		memmove( mFirst + index, mFirst + index + 1,
			( mMaxCount - index - 1 ) * sizeof( void * ) );
	} else {
		mFirst[ index ] = NULL;
	}

	return ret;
}

const void * SP_ArrayList :: getItem( int index ) const
{
	const void * ret = NULL;

	if( LAST_INDEX == index ) index = mCount - 1;
	if( index < 0 || index >= mCount ) return ret;

	ret = mFirst[ index ];

	return ret;
}

//-------------------------------------------------------------------

SP_CircleQueue :: SP_CircleQueue()
{
	mMaxCount = 8;
	mEntries = (void**)malloc( sizeof( void * ) * mMaxCount );

	mHead = mTail = mCount = 0;
}

SP_CircleQueue :: ~SP_CircleQueue()
{
	free( mEntries );
	mEntries = NULL;
}

void SP_CircleQueue :: push( void * item )
{
	if( mCount >= mMaxCount ) {
		mMaxCount = ( mMaxCount * 3 ) / 2 + 1;
		void ** newEntries = (void**)malloc( sizeof( void * ) * mMaxCount );

		unsigned int headLen = 0, tailLen = 0;
		if( mHead < mTail ) {
			headLen = mTail - mHead;
		} else {
			headLen = mCount - mTail;
			tailLen = mTail;
		}

		memcpy( newEntries, &( mEntries[ mHead ] ), sizeof( void * ) * headLen );
		if( tailLen ) {
			memcpy( &( newEntries[ headLen ] ), mEntries, sizeof( void * ) * tailLen );
		}

		mHead = 0;
		mTail = headLen + tailLen;

		free( mEntries );
		mEntries = newEntries;
	}

	mEntries[ mTail++ ] = item;
	mTail = mTail % mMaxCount;
	mCount++;
}

void * SP_CircleQueue :: pop()
{
	void * ret = NULL;

	if( mCount > 0 ) {
		ret = mEntries[ mHead++ ];
		mHead = mHead % mMaxCount;
		mCount--;
	}

	return ret;
}

void * SP_CircleQueue :: top()
{
	return mCount > 0 ? mEntries[ mHead ] : NULL;
}

int SP_CircleQueue :: getLength()
{
	return mCount;
}

//-------------------------------------------------------------------

SP_BlockingQueue :: SP_BlockingQueue()
{
	mQueue = new SP_CircleQueue();
	pthread_mutex_init( &mMutex, NULL );
	pthread_cond_init( &mCond, NULL );
}

SP_BlockingQueue :: ~SP_BlockingQueue()
{
	delete mQueue;
	pthread_mutex_destroy( &mMutex );
	pthread_cond_destroy( &mCond );
}

void SP_BlockingQueue :: push( void * item )
{
	pthread_mutex_lock( &mMutex );

	mQueue->push( item );

	if( mQueue->getLength() == 1 ) {
		pthread_cond_signal( &mCond );
	}

	pthread_mutex_unlock( &mMutex );
}

void * SP_BlockingQueue :: pop()
{
	void * ret = NULL;

	pthread_mutex_lock( &mMutex );

	if( mQueue->getLength() == 0 ) {
		pthread_cond_wait( &mCond, &mMutex );
	}

	ret = mQueue->pop();

	pthread_mutex_unlock( &mMutex );

	return ret;
}

void * SP_BlockingQueue :: top()
{
	void * ret = NULL;

	pthread_mutex_lock( &mMutex );

	ret = mQueue->top();

	pthread_mutex_unlock( &mMutex );

	return ret;
}

int SP_BlockingQueue :: getLength()
{
	int len = 0;

	pthread_mutex_lock( &mMutex );

	len = mQueue->getLength();

	pthread_mutex_unlock( &mMutex );

	return len;
}


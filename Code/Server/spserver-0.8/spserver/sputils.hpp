/*
 * Copyright 2007 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#ifndef __sputils_hpp__
#define __sputils_hpp__

#include <pthread.h>

class SP_ArrayList {
public:
	static const int LAST_INDEX;

	SP_ArrayList( int initCount = 2 );
	virtual ~SP_ArrayList();

	int getCount() const;
	int append( void * value );
	const void * getItem( int index ) const;
	void * takeItem( int index );

private:
	SP_ArrayList( SP_ArrayList & );
	SP_ArrayList & operator=( SP_ArrayList & );

	int mMaxCount;
	int mCount;
	void ** mFirst;
};

class SP_CircleQueue {
public:
	SP_CircleQueue();
	virtual ~SP_CircleQueue();

	void push( void * item );
	void * pop();
	void * top();
	int getLength();

private:
	void ** mEntries;
	unsigned int mHead;
	unsigned int mTail;
	unsigned int mCount;
	unsigned int mMaxCount;
};

class SP_BlockingQueue {
public:
	SP_BlockingQueue();
	virtual ~SP_BlockingQueue();

	// non-blocking
	void push( void * item );

	// blocking until can pop
	void * pop();

	// non-blocking, if empty then return NULL
	void * top();

	// non-blocking
	int getLength();

private:
	SP_CircleQueue * mQueue;
	pthread_mutex_t mMutex;
	pthread_cond_t mCond;
};

#endif


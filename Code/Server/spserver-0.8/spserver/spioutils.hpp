/*
 * Copyright 2007 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#ifndef __spioutils_hpp__
#define __spioutils_hpp__

#include <netinet/in.h>

class SP_IOUtils {
public:
	static void inetNtoa( in_addr * addr, char * ip, int size );

	static int setNonblock( int fd );

	static int setBlock( int fd );

	static int tcpListen( const char * ip, int port, int * fd, int blocking = 1 );

private:
	SP_IOUtils();
};

#endif


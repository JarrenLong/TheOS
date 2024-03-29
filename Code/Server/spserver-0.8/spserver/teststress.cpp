/*
 * Copyright 2007 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* For inet_ntoa. */
#include <arpa/inet.h>

#include "config.h"
#include "event.h"

static const char * gHost = "127.0.0.1";
static int gPort = 5555;
static int gMsgs = 10;
static int gClients = 10;

static time_t gStartTime = 0;

struct SP_TestClient {
	int mFd;
	struct event mReadEvent;
	struct event mWriteEvent;
	int mSendMsgs;
	int mRecvMsgs;
	char mBuffer[ 512 ];
};

void showUsage( const char * program )
{
	printf( "Stress Test Tools for spserver example -- testchat\n" );
	printf( "Usage: %s [-h <host>] [-p <port>] [-c <clients>] [-m <messages>]\n", program );
	printf( "\t-h default is %s\n", gHost );
	printf( "\t-p default is %d\n", gPort );
	printf( "\t-c how many clients, default is %d\n", gClients );
	printf( "\t-m messages per client, default is %d\n", gMsgs );
	printf( "\n" );
}

void close_read( SP_TestClient * client )
{
	//fprintf( stderr, "#%d close read\n", client->mFd );
	event_del( &client->mReadEvent );
	gClients--;
}

void close_write( SP_TestClient * client )
{
	//fprintf( stderr, "#%d close write\n", client->mFd );
	event_del( &client->mWriteEvent );
}

void close_client( SP_TestClient * client )
{
	close_write( client );
	close_read( client );
}

void on_read( int fd, short events, void *arg )
{
	SP_TestClient * client = ( SP_TestClient * ) arg;

	if( EV_READ & events ) {
		int len = read( fd, client->mBuffer, sizeof( client->mBuffer ) );
		if( len <= 0 ) {
			if( len < 0 && EINTR != errno && EAGAIN != errno ) {
				fprintf( stderr, "#%d on_read error\n", fd );
			}
			close_client( client );
		} else {
			for( int i = 0; i < len; i++ ) {
				//if( 10 == fd ) printf( "%c", client->mBuffer[i] );
				if( '\n' == client->mBuffer[i] ) client->mRecvMsgs++;
			}
		}
	} else {
		fprintf( stderr, "#%d on_read timeout\n", fd );
		close_client( client );
	}
}

void on_write( int fd, short events, void *arg )
{
	SP_TestClient * client = ( SP_TestClient * ) arg;

	if( EV_WRITE & events ) {
		client->mSendMsgs++;

		if( client->mSendMsgs >= gMsgs ) {
			snprintf( client->mBuffer, sizeof( client->mBuffer ), "quit\r\n" );
		} else {
			snprintf( client->mBuffer, sizeof( client->mBuffer ),
				"mail #%d, It's good to see how people hire; "
				"that tells us how to market ourselves to them.\r\n", client->mSendMsgs );
		}

		int len = write( fd, client->mBuffer, strlen( client->mBuffer ) );

		if( len <= 0 && EINTR != errno && EAGAIN != errno ) {
			fprintf( stderr, "#%d on_write error\n", fd );
			close_client( client );
		} else {
			if( client->mSendMsgs >= gMsgs ) close_write( client );
		}
	} else {
		fprintf( stderr, "#%d on_write timeout\n", fd );
		close_client( client );
	}
}

int main( int argc, char * argv[] )
{
	extern char *optarg ;
	int c ;

	while( ( c = getopt ( argc, argv, "h:p:c:m:v" )) != EOF ) {
		switch ( c ) {
			case 'h' :
				gHost = optarg;
				break;
			case 'p':
				gPort = atoi( optarg );
				break;
			case 'c' :
				gClients = atoi ( optarg );
				break;
			case 'm' :
				gMsgs = atoi( optarg );
				break;
			case 'v' :
			case '?' :
				showUsage( argv[0] );
				exit( 0 );
		}
	}

	signal( SIGPIPE, SIG_IGN );

        event_init();

	SP_TestClient * clientList = (SP_TestClient*)calloc( gClients, sizeof( SP_TestClient ) );

	struct sockaddr_in sin;
	memset( &sin, 0, sizeof(sin) );
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr( gHost );
	sin.sin_port = htons( gPort );

	int totalClients = gClients;

	printf( "Create %d connections to server, it will take some minutes to complete.\n", gClients );
	for( int i = 0; i < gClients; i++ ) {
		SP_TestClient * client = clientList + i;

		client->mFd = socket( AF_INET, SOCK_STREAM, 0 );
		if( client->mFd < 0 ) {
			fprintf(stderr, "socket failed\n");
			return -1;
		}

		if( connect( client->mFd, (struct sockaddr *)&sin, sizeof(sin) ) != 0) {
			fprintf(stderr, "connect failed\n");
			return -1;
		}

		event_set( &client->mWriteEvent, client->mFd, EV_WRITE | EV_PERSIST, on_write, client );
		event_add( &client->mWriteEvent, NULL );

		event_set( &client->mReadEvent, client->mFd, EV_READ | EV_PERSIST, on_read, client );
		event_add( &client->mReadEvent, NULL );

		if( 0 == ( i % 10 ) ) write( fileno( stdout ), ".", 1 );
	}

	time( &gStartTime );

	struct timeval startTime, stopTime;
	struct timezone startZone, stopZone;

	gettimeofday( &startTime, &startZone );

	time_t lastInfoTime = time( NULL );

	// start event loop until all clients are exit
	while( gClients > 0 ) {
		event_loop( EVLOOP_ONCE );

		if( time( NULL ) - lastInfoTime > 5 ) {
			time( &lastInfoTime );
			printf( "waiting for %d client(s) to exit\n", gClients );
		}
	}

	gettimeofday( &stopTime, &stopZone );

	// show result
	printf( "\n\nTest result :\n" );
	printf( "Clients : %d, Messages Per Client : %d\n", totalClients, gMsgs );
	printf( "ExecTimes: %.6f seconds\n\n",
		(double) ( 1000000 * ( stopTime.tv_sec - startTime.tv_sec )
			+ ( stopTime.tv_usec - startTime.tv_usec ) )
		/ 1000000 );

	printf( "client\tSend\tRecv\n" );
	int totalSend = 0, totalRecv = 0;
	for( int i = 0; i < totalClients; i++ ) {
		SP_TestClient * client = clientList + i;

		//printf( "client#%d : %d\t%d\n", i, client->mSendMsgs, client->mRecvMsgs );

		totalSend += client->mSendMsgs;
		totalRecv += client->mRecvMsgs;

		close( client->mFd );
	}

	printf( "total : %d\t%d\n", totalSend, totalRecv );

	free( clientList );

	return 0;
}


#include "Socket.h"

long Socket::refCount = 0;
long ISmart::refCount = 0;

Socket::Socket() {
    WSADATA wd;
    if ( ++refCount == 1 ) {
        ::WSAStartup( 0x0101, &wd );
    }
}

Socket::~Socket() {
    Close();
    if ( --refCount == 0 ) {
        ::WSACleanup();
    }
}

bool Socket::Create( int af ) {
    m_hSocket = ::socket( af, SOCK_STREAM, IPPROTO_TCP );
    if ( m_hSocket == INVALID_SOCKET ) {
        return false;
    } else {
        return true;
    }
}

bool Socket::Connect( const char *pszHost, int nPort ) {
    unsigned long ulAddr = 0;
    hostent *pEnt = ::gethostbyname( pszHost );
    SOCKADDR_IN addr;
    if ( pEnt == 0 ) {
        ulAddr = ::inet_addr( pszHost );
        if ( ulAddr == INADDR_NONE ) {
            return false;
        } else {
            addr.sin_family = AF_INET;
        }
    } else {
        memcpy( &ulAddr, pEnt->h_addr_list[0], sizeof( long ) );
        addr.sin_family = pEnt->h_addrtype;
    }
    addr.sin_addr.s_addr = ulAddr;
    addr.sin_port = ::htons( nPort );
    memset( addr.sin_zero, 0, sizeof( addr.sin_zero ) );
    if ( ::connect( m_hSocket, (const sockaddr *)&addr, sizeof( SOCKADDR_IN ) ) == SOCKET_ERROR ) {
        return false;
    } else {
        return true;
    }
}

bool Socket::Bind( int nLocalPort ) {
    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = ::htons( nLocalPort );
    memset( addr.sin_zero, 0, sizeof( addr.sin_zero ) );
    if ( ::bind( m_hSocket, (const sockaddr *)&addr, sizeof( SOCKADDR_IN ) ) == SOCKET_ERROR ) {
        return false;
    } else {
        return true;
    }
}

bool Socket::Accept( Socket *pSocket ) {
    if ( pSocket == 0 ) {
        return false;
    }
    SOCKADDR_IN addr;
    int len = sizeof( SOCKADDR_IN );
    memset( &addr, 0, sizeof( SOCKADDR_IN ) );
    pSocket->m_hSocket = ::accept( m_hSocket, (sockaddr*)&addr, &len );
    if ( pSocket->m_hSocket == INVALID_SOCKET ) {
        return false;
    } else {
        return true;
    }
}

bool Socket::Listen( int nBackLog ) {
    if ( ::listen( m_hSocket, nBackLog ) == SOCKET_ERROR ) {
        return false;
    } else {
        return true;
    }
}

int Socket::Send( const void *pData, int nDataLen, int nFlags ) {
    return ::send( m_hSocket, (const char *)pData, nDataLen, nFlags );
}

int Socket::SendText( const char *pszText ) {
    return Send( pszText, strlen( pszText ) );
}

int Socket::Recv( void *pData, int nDataLen, int nFlags ) {
    return ::recv( m_hSocket, (char *)pData, nDataLen, nFlags );
}

int Socket::RecvLine( char *pszBuf, int nLen) {
    int nCount = 0;
    int nRdLen = 0;
    char ch = 0;

    while ( ch != '\n' && nCount < nLen ) {
        nRdLen = Recv( &ch, 1 );
        if ( nRdLen == 0 || nRdLen == SOCKET_ERROR ) {
            nCount = 0;
            break;
        }
        if ( ch != '\n' && ch != '\r' ) {
            pszBuf[nCount] = ch;
            nCount++;
        }
    }
    if ( nCount != 0 ) {
        pszBuf[nCount] = 0;
    }
    return nCount ? nCount : nRdLen;
}

bool Socket::Shutdown( int nHow ) {
    return ::shutdown( m_hSocket, nHow ) == SOCKET_ERROR ? false : true;
}

bool Socket::Close( void ) {
    return ::closesocket( m_hSocket ) == SOCKET_ERROR ? false : true;
}

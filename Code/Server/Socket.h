#ifndef SOCKET_H
#define SOCKET_H

typedef void (*Handler)(void *params);

class ISmart {
public:
    ISmart() {
        refCount++;
    }
    virtual ~ISmart() {
        refCount--;
    }
protected:
    virtual void OnAlloc(Handler *oA) {}
    virtual void OnRelease(Handler *oR) {}
private:
    static long refCount;
};

class Socket : private ISmart {
public:
    Socket();
    ~Socket();

    //bool Create( int af = AF_INET );
    bool Connect( const char *pszHost, int nPort );
    bool Bind( int nLocalPort );
    bool Accept( Socket *pSocket );
    //bool Listen( int nBacklog = SOMAXCONN );
    int Send( const void *pData, int nDataLen, int nFlags = 0 );
    int SendText( const char *pszText );
    int Recv( void *pData, int nDataLen, int nFlags = 0 );
    int RecvLine( char *pszBuf, int nLen);
    bool Shutdown( int nHow );
    bool Close( void );

    //SOCKET m_hSocket;
private:
    static long refCount;
};

#endif

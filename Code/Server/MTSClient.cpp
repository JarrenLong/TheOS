#include <iostream>
#include <windows.h>
#include <winsock2.h>

CWinApp theApp;

int EndFile(char *buff, int len) ;
int NoFile(char *buff, int len) ;

using namespace std;

int main(int argc, char *argv[], char* envp[]) {
    const char* servername="127.0.0.1";
    if (argc<2) {
        cout << "Usage :- mtsclient [file1] [file2] [file3] ....\r\n";
        return 0;
    }
    WSADATA wsaData;
    struct hostent *hp;
    unsigned int addr;
    struct sockaddr_in server;
    int wsaret=WSAStartup(0x101,&wsaData);
    if (wsaret)
        return 0;
    SOCKET conn;
    conn=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (conn==INVALID_SOCKET)
        return 0;
    if (inet_addr(servername)==INADDR_NONE) {
        hp=gethostbyname(servername);
    } else {
        addr=inet_addr(servername);
        hp=gethostbyaddr((char*)&addr,sizeof(addr),AF_INET);
    }
    if (hp==NULL) {
        closesocket(conn);
        return 0;
    }
    server.sin_addr.s_addr=*((unsigned long*)hp->h_addr);
    server.sin_family=AF_INET;
    server.sin_port=htons(20248);
    if (connect(conn,(struct sockaddr*)&server,sizeof(server))) {
        closesocket(conn);
        return 0;
    }
    char buff[512];
    int z;
    z=recv(conn,buff,512,0);
    buff[z]=0;
    strcpy(buff,"auth passwd\r\n");
    send(conn,buff,strlen(buff),0);
    z=recv(conn,buff,512,0);
    buff[z]=0;
    for (int j=1;j<argc;j++) {
        sprintf(buff,"file %s\r\n",argv[j]);
        send(conn,buff,strlen(buff),0);
        CFile f;
        char *fname;
        char fname2[256];
        if (!(fname=strrchr(argv[j],'\\')))
            strcpy(fname2,argv[j]);
        else
            strcpy(fname2,fname+1);

        f.Open(fname2,CFile::modeCreate | CFile::modeWrite);
        bool first=true;
        bool second=false;
        while (true) {
            z=recv(conn,buff,128,0);
            if (z==SOCKET_ERROR) {
                cout << "\r\n\r\nsocket error socket error socket error\r\n";
                break;
            }
            if (second) {
                if (NoFile(buff,128)<128) {
                    cout << "File " << argv[j] << " not found on server\r\n";
                    break;
                }
                second=false;
            }
            if (first) {
                if (NoFile(buff,128)<128) {
                    cout << "File " << argv[j] << " not found on server\r\n";
                    break;
                }
                first=false;
                second=true;
            }

            int b;
            if ((b=EndFile(buff,z))<z) {
                f.Write(buff,b);
                cout << fname2 << " has been saved.\r\n";
                break;
            }
            f.Write(buff,z);
        }
        f.Close();
    }
    closesocket(conn);
    WSACleanup();
    return 0;

}

int EndFile(char *buff, int len) {
    int pos=len;
    for (int u=0;u<(len-4);u++) {
        if (buff[u]=='#')
            if (buff[u+1]=='F')
                if (buff[u+2]=='i')
                    if (buff[u+3]=='l')
                        if (buff[u+4]=='e') {
                            pos=u;
                            break;
                        }
    }
    return pos;
}

int NoFile(char *buff, int len) {
    int pos=len;
    for (int u=0;u<(len-4);u++) {
        if (buff[u]=='!')
            if (buff[u+1]=='F')
                if (buff[u+2]=='i')
                    if (buff[u+3]=='l')
                        if (buff[u+4]=='e') {
                            pos=u;
                            break;
                        }
    }
    return pos;
}

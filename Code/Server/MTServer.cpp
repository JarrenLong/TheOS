#include <windows.h>
#include <iostream>
#include <conio.h>
#include <winsock2.h>

CWinApp theApp;
SOCKET server;

#define AfxBeginThread(x,y) {}

using namespace std;

UINT  MTServerThread(LPVOID pParam);
UINT  ClientThread(LPVOID pParam);
bool ParseCmd(char *str, std::string& cmd, std::string& params);
bool SendFile(SOCKET s, std::string fname);

int main() {
    int nRetCode = 0;
    std::cout << "Press ESCAPE to terminate program\r\n";
    AfxBeginThread(MTServerThread,0);
    while (_getch()!=27);
    closesocket(server);
    WSACleanup();
    return nRetCode;
}

UINT  MTServerThread(LPVOID pParam) {
    WSADATA wsaData;
    sockaddr_in local;
    int wsaret=WSAStartup(0x101,&wsaData);
    if (wsaret!=0) {
        return 0;
    }
    local.sin_family=AF_INET;
    local.sin_addr.s_addr=INADDR_ANY;
    local.sin_port=htons((u_short)20248);
    server=socket(AF_INET,SOCK_STREAM,0);
    if (server==INVALID_SOCKET) {
        return 0;
    }
    if (bind(server,(sockaddr*)&local,sizeof(local))!=0) {
        return 0;
    }
    if (listen(server,10)!=0) {
        return 0;
    }
    SOCKET client;
    sockaddr_in from;
    int fromlen=sizeof(from);
    while (true) {
        client=accept(server,
                      (struct sockaddr*)&from,&fromlen);
        AfxBeginThread(ClientThread,(LPVOID)client);
    }
    return 0;
}

UINT  ClientThread(LPVOID pParam) {
    char buff[512];
    std::string cmd;
    std::string params;
    int n;
    int x;
    bool auth=false;
    SOCKET client=(SOCKET)pParam;
    strcpy(buff,"#Server Ready.\r\n");
    send(client,buff,strlen(buff),0);
    while (true) {
        n=recv(client,buff,512,0);
        if (n==SOCKET_ERROR )
            break;
        buff[n]=0;
        if (ParseCmd(buff,cmd,params)) {
            if (cmd=="QUIT")
                break;
            if (cmd=="AUTH") {
                if (params=="passwd") {
                    auth=true;
                    strcpy(buff,"#You are logged in.\r\n");
                } else {
                    strcpy(buff,"!Bad password.\r\n");
                }
                send(client,buff,strlen(buff),0);
            }
            if (cmd=="FILE") {
                if (auth) {
                    if (SendFile(client,params))
                        sprintf(buff,"#File %s sent successfully.\r\n",params);
                    else
                        sprintf(buff,"!File %s could not be opened.\r\n",params);
                    x=send(client,buff,strlen(buff),0);
                } else {
                    strcpy(buff,"!You are not logged in.\r\n");
                    send(client,buff,strlen(buff),0);
                }
            }
        } else {
            strcpy(buff,"!Invalid command.\r\n");
            send(client,buff,strlen(buff),0);
        }
    }
    closesocket(client);
    return 0;
}

bool ParseCmd(char *str, std::string& cmd, std::string& params) {
    int n;
    std::string tmp=str;
    tmp.TrimLeft();
    tmp.TrimRight();
    if ((n=tmp.Find(' '))==-1) {
        tmp.MakeUpper();
        if (tmp!="QUIT")
            return false;
        cmd=tmp;
        return true;
    }
    cmd=tmp.Left(n);
    params=tmp.Mid(n+1);
    cmd.MakeUpper();
    if ((cmd!="AUTH") && (cmd!="FILE"))
        return false;
    return true;
}

bool SendFile(SOCKET s, std::string fname) {
    CFile f;
    bool p=f.Open(fname,CFile::modeRead);
    char buff[1024];
    int y;
    int x;
    if (!p)
        return false;
    while (true) {
        y=f.Read(buff,1024);
        x=send(s,buff,y,0);
        if (y<1024) {
            f.Close();
            break;
        }
    }
    return true;
}

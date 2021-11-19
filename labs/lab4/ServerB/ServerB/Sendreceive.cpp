#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include "error.h"
#include "Winsock2.h"
#include "service.h"
#include "sendreceive.h"

#define CALL_SIGN "hello"
#define MAX_MESSAGE 50
#define CLOSE "close"

using namespace std;

bool  GetRequestFromClient(SOCKET sS, char* name, short port, SOCKADDR_IN* client_addr, int* flen)
{
    int lengthBuffer = 0;                                //буффер для сообщения
    SOCKADDR_IN temp;

    lengthBuffer = recvfrom(sS, name, MAX_MESSAGE, 0, (sockaddr*)&temp, flen);
    if (lengthBuffer == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAETIMEDOUT)
            return false;
        else
            throw  SetErrorMsgText("recvfrom: ", WSAGetLastError());
    }
    std::cout << name << std::endl;
    if (strcmp(name, CALL_SIGN) == 0)
    {
        memset(client_addr, 0, sizeof(*client_addr));
        cout << "Confirmed" << endl;
        client_addr->sin_addr = temp.sin_addr;
        client_addr->sin_port = temp.sin_port;
        client_addr->sin_family = temp.sin_family;
        cout << "client (ip: " << inet_ntoa(client_addr->sin_addr) << \
            ", port: " << client_addr->sin_port << "), message: " << name << endl;
            return true;
    }
    else if (strcmp(name, CLOSE) == 0)
    {
        PutAnswerToClient(sS, (char*)"Server will stopped", client_addr, flen);
        cout << "client (ip: " << inet_ntoa(client_addr->sin_addr) << \
            ", port: " << client_addr->sin_port << "), message: " << name << endl;
            destroy(&sS);
    }
    else {
        cout << "Rejected" << endl;
        PutAnswerToClient(sS, (char*)"", client_addr, flen);
        return false;
    }
}

bool  PutAnswerToClient(SOCKET sock, char* name, SOCKADDR_IN* client_addr, int* lto)
{
    int lengthBuffer = 0;                                //буффер для сообщения

    lengthBuffer = sendto(sock, name, MAX_MESSAGE, 0, (sockaddr*)client_addr, *lto);

    if (lengthBuffer == SOCKET_ERROR)
        return false;
    else
        return true;

}
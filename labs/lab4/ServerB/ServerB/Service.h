#pragma once
#include <Winsock2.h>

void init
(SOCKET*,                    //�����
    WSADATA*,                   //��������� ��� ������������� lib, "WS2_32.lib"
    SOCKADDR_IN*, int*           //
);

void initaddr
(
    SOCKADDR_IN* servaddr,      //socket
    int port,                   //port
    unsigned short address,     //address
    int typeprotocol            //��� ��������� 4/6
);

void destroy
(
    SOCKET* sock                 //socket
);
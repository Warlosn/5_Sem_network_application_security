#pragma once
#include <Winsock2.h>

bool  GetRequestFromClient(     //��������� �� ������� � �������� ���������
    SOCKET           sock,  //����� �������
    char* name,  //[in] �������� �������
    short            port,  //[in] ����� �������������� �����
    SOCKADDR_IN* from,  //[out] ��������� �� SOCKADDR_IN
    int* flen   //[out] ��������� �� ������ from
);

bool  PutAnswerToClient(       //����� �������
    SOCKET           sock, //����� �������
    char* name, //[in] �������� �������
    SOCKADDR_IN* to,   //[in] ��������� �� SOCKADDR_IN
    int* lto   //[in] ��������� �� ������ from
);

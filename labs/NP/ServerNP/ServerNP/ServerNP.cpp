#include <iostream>
#include <clocale>
#include <ctime>

#include "ErrorMsgText.h"
#include "Windows.h"

#define NAME L"\\\\.\\pipe\\Tube"

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");

    //LocalAlloc - allocate bytes from the heap (flags - fixed memory, init zero, size)
    //discretionary access control list
    SECURITY_DESCRIPTOR* m_pSecDesc = (SECURITY_DESCRIPTOR*)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    SECURITY_ATTRIBUTES m_pSecAttrib = { sizeof(SECURITY_ATTRIBUTES), m_pSecDesc, TRUE }; //true specifies whether the returned handle is inherited when a new process is created.
    InitializeSecurityDescriptor(m_pSecDesc, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(m_pSecDesc, TRUE, (PACL)NULL, FALSE);
    //true - 3,4 params are used; pointer to ACL structure, NULL - allow access; the DACL has been explicitly specified by a user

    HANDLE sH; 
    DWORD lp;
    char buf[50];

    try {
        cout << "ServerNP\n\n";

        if ((sH = CreateNamedPipe(NAME,
            PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT,
            1, NULL, NULL, INFINITE, &m_pSecAttrib)) == INVALID_HANDLE_VALUE) {
            throw SetPipeError("CreateNamedPipe: ", GetLastError());
        }

        cout << "Waiting for connect...\n";

        while (true) {
            if (!ConnectNamedPipe(sH, NULL)) {
                throw SetPipeError("ConnectNamedPipe: ", GetLastError());
            }
            while (true) {
                if (ReadFile(sH, buf, sizeof(buf), &lp, NULL)) {

                    if (strcmp(buf, "STOP") == 0) {
                        cout << endl;
                        break;
                    }
                    cout << buf << endl;
                    if (WriteFile(sH, buf, sizeof(buf), &lp, NULL)) {
                        if (strstr(buf, "ClientNPct")) {
                            break;
                        }
                    }
                    else {
                        throw SetPipeError("WriteFile: ", GetLastError());
                    }
                }
                else {
                    throw SetPipeError("ReadFile: ", GetLastError());
                }
            }
            if (!DisconnectNamedPipe(sH)) {
                throw SetPipeError("DisconnectNamedPipe: ", GetLastError());
            }
        }

        if (!CloseHandle(sH)) {
            throw SetPipeError("CloseHandle: ", GetLastError());
        }

        system("pause");
    }
    catch (string ErrorPipeText) {
        cout << endl << ErrorPipeText;
    }
}

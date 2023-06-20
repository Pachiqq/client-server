#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <map>
#include <ctime>
using namespace std;

int main()
{
    WSADATA wsaData;
    ADDRINFO hints;
    ADDRINFO* addrResult = NULL;
    SOCKET ClientSoket = INVALID_SOCKET;
    SOCKET ListenSoket = INVALID_SOCKET;

    const char* sendBuffer = "SendScr";
    int SaveConnection[512];
    char recvBuffer[512];
    map<int, string> Save;
    int result, Connection=0;

    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        cout << "WSAStartup Failed, result = " << result << endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;


    result = getaddrinfo("NULL", "123", &hints, &addrResult);
    if (result != 0)
    {
        cout << "getaddrinfo failed with error: " << result << endl;
        WSACleanup();
        return 1;
    }

    ListenSoket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (ListenSoket == INVALID_SOCKET)
    {
        cout << "Socket creation failed" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    result = bind(ListenSoket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        cout << "Binding socket failed" << endl;
        closesocket(ListenSoket);
        ListenSoket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = listen(ListenSoket, SOMAXCONN);
    if (result == SOCKET_ERROR)
    {
        cout << "Listening socket failed" << endl;
        closesocket(ListenSoket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    thread t1([&]()
        {
            while (true) {
                ClientSoket = accept(ListenSoket, NULL, NULL);
                if (ClientSoket == INVALID_SOCKET)
                {
                    cout << "Accepting socket failed" << endl;
                    closesocket(ListenSoket);
                    freeaddrinfo(addrResult);
                    WSACleanup();
                    return 1;
                }
                SaveConnection[Connection] = ClientSoket;
                Connection++;
            }
        });
    thread t2([&]()
        {
    do
    {
        ZeroMemory(recvBuffer, 512);
        result = recv(ClientSoket, recvBuffer, 512, 0);
        if (result > 0)
        {
            cout << "Received data: " << recvBuffer << endl;
            Save[SaveConnection[Connection]] = recvBuffer;
            result = send(ClientSoket, sendBuffer, (int)strlen(sendBuffer), 0);
            if (result == SOCKET_ERROR)
            {
                cout << "Failed to send data back" << endl;
                closesocket(ClientSoket);
                freeaddrinfo(addrResult);
                WSACleanup();
                return 1;
            }
            else if (result == 0)
            {
                cout << "Connecting closing" << endl;
                time_t now = time(0);
                char* dt = ctime(&now);
                Save[SaveConnection[Connection]] += dt;
            }
            else
            {
                cout << "recv failed with error" << endl;
                closesocket(ClientSoket);
                freeaddrinfo(addrResult);
                WSACleanup();
                return 1;
            }
        }
    } while (result > 0);

    result = shutdown(ClientSoket, SD_SEND);
    if (result == SOCKET_ERROR)
    {
        cout << "shutdown client socket failed" << endl;
        closesocket(ClientSoket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    closesocket(ClientSoket);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 0;
        });
    thread t3([&]()
        {
            string cashe;
            while (cashe != "close") {
                cout << "show connection history?";
                cin >> cashe;
                if (cashe == "yes")
                {
                    for (int i = 0; i <= Connection; i++) {
                        cout << "Socket: " << SaveConnection[i] << " info: " << Save[SaveConnection[Connection]]<<endl;
                    }
                }
            }
        });
}

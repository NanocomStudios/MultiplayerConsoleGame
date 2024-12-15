#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <time.h>

#include "screenCtrl.h"
#include "client.h"
#include "game.h"
#include "io.h"
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 2
#define DEFAULT_PORT "27015"

WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo* result = NULL,
    * ptr = NULL,
    hints;
const char* sendbuf = "this is a test";
char recvbuf[DEFAULT_BUFLEN];
int iResult;
int recvbuflen = DEFAULT_BUFLEN;

int iSendResult;

bool isReceived = false;

int __cdecl client()
{
    char inputString[] = "12700000000127015";

    getIP(inputString);

    system("cls");
    consoleColorSet(95);
    drawPlayField();
    consoleColorSet(37);
    drawMsgBox();
    consoleColorSet(104);
    moveCsr(12, 26);
    std::cout << "         Connecting...";

    consoleColorSet(40);

    int numberCount = 0;
    char zeroCounter = 0;

    char ipaddr[16];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (inputString[(i * 3) + j] != '0') {
                ipaddr[numberCount] = inputString[(i * 3) + j];
                numberCount++;
            }
            else {
                if (zeroCounter == 2) {
                    ipaddr[numberCount] = '0';
                    numberCount++;
                    zeroCounter = 0;
                }
                else {
                    zeroCounter++;
                }
            }
        }
        ipaddr[numberCount] = '.';
        numberCount++;
    }

    for (int j = 0; j < 3; j++) {
        if (inputString[9 + j] != '0') {
            ipaddr[numberCount] = inputString[9 + j];
            numberCount++;
        }
    }
    ipaddr[numberCount] = 0;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(ipaddr, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    /*iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }*/

    // Receive until the peer closes the connection
    std::thread t1(receive);
    t1.detach();

    char inp[2];

    system("cls");
    consoleColorSet(94);
    drawPlayField();
    consoleColorSet(37);
    Player player;
    Opponent opponent;
    int opponentPosition = 40;
    char playerPosition = 40;

    player.draw(playerPosition);
    clock_t playerClock = clock();

    while (1) {
        Sleep(1);

        if ((GetKeyState(0x44) == (-128)) || (GetKeyState(0x44) == (-127))) { // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
            if ((clock() - playerClock) > PlayerMoveSpeed) {
                if (playerPosition < 77) {
                    playerPosition++;
                }
                inp[0] = playerPosition;
                player.draw(playerPosition);
                playerClock = clock();
            }
        }
        if ((GetKeyState(0x41) == (-128)) || (GetKeyState(0x41) == (-127))) {

            if ((clock() - playerClock) > PlayerMoveSpeed) {
                if (playerPosition > 6) {
                    playerPosition--;
                }
                inp[0] = playerPosition;
                player.draw(playerPosition);
                playerClock = clock();
            }
        }
        

        iSendResult = send(ConnectSocket, inp, 1, 0);
        if (isReceived == true) {
            opponentPosition = recvbuf[0];
            if ((opponentPosition > 77) || (opponentPosition < 6)) {
                opponentPosition = 40;
            }
            opponent.draw(opponentPosition);
            isReceived = false;
        }
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}


void receive() {
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            isReceived = true;
        }
        //printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);
}
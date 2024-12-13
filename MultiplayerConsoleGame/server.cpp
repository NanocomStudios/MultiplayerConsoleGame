#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <thread>

#include "screenCtrl.h"
#include "game.h"
#include "server.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 2
#define DEFAULT_PORT "27015"

WSADATA SwsaData;
int SiResult;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

struct addrinfo* Sresult = NULL;
struct addrinfo Shints;

int SiSendResult;
char Srecvbuf[DEFAULT_BUFLEN];
int Srecvbuflen = DEFAULT_BUFLEN;

bool SisReceived = false;

int __cdecl server(void)
{
    // Initialize Winsock
    SiResult = WSAStartup(MAKEWORD(2, 2), &SwsaData);
    if (SiResult != 0) {
        printf("WSAStartup failed with error: %d\n", SiResult);
        return 1;
    }

    ZeroMemory(&Shints, sizeof(Shints));
    Shints.ai_family = AF_INET;
    Shints.ai_socktype = SOCK_STREAM;
    Shints.ai_protocol = IPPROTO_TCP;
    Shints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    SiResult = getaddrinfo(NULL, DEFAULT_PORT, &Shints, &Sresult);
    if (SiResult != 0) {
        printf("getaddrinfo failed with error: %d\n", SiResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(Sresult->ai_family, Sresult->ai_socktype, Sresult->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(Sresult);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    SiResult = bind(ListenSocket, Sresult->ai_addr, (int)Sresult->ai_addrlen);
    if (SiResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(Sresult);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(Sresult);

    system("cls");
    consoleColorSet(94);
    drawPlayField();
    consoleColorSet(37);
    drawMsgBox();
    consoleColorSet(104);
    moveCsr(11, 26);
    std::cout << "  Waiting for a Connection";
    
    moveCsr(14, 34);
    std::cout << "PORT:  " << DEFAULT_PORT;

    SiResult = listen(ListenSocket, SOMAXCONN);
    if (SiResult == SOCKET_ERROR) {
        printf("      listen failed.      ");
        consoleColorSet(40);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("      accept failed.      ");
        consoleColorSet(40);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    consoleColorSet(40);
    // No longer need server socket
    closesocket(ListenSocket);

    std::thread t1(Sreceive);
    t1.detach();
    
    char inp[2] = {0,0};

    char playerPosition = 40;

    system("cls");
    drawPlayField();

    Player player;
    Opponent opponent;
    int opponentPosition = 40;

    clock_t playerClock = clock();

    player.draw(playerPosition);
    opponent.draw(opponentPosition);

    while (1) {
        Sleep(1);

        if ((GetKeyState(0x27) == (-128)) || (GetKeyState(0x27) == (-127))) { // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
            if ((clock() - playerClock) > PlayerMoveSpeed) {
                if (playerPosition < 77) {
                    playerPosition++;
                }
                inp[0] = playerPosition;
                SiSendResult = send(ClientSocket, inp, DEFAULT_BUFLEN, 0);
                playerClock = clock();
            }
        }
        if ((GetKeyState(0x25) == (-128)) || (GetKeyState(0x25) == (-127))) {

            if ((clock() - playerClock) > PlayerMoveSpeed) {
                if (playerPosition > 6) {
                    playerPosition--;
                }
                inp[0] = playerPosition;
                SiSendResult = send(ClientSocket, inp, DEFAULT_BUFLEN, 0);
                playerClock = clock();
            }
        }

        player.draw(playerPosition);

        if (SiSendResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            break;
        }

        if (SisReceived == true) {
            opponentPosition = Srecvbuf[0];
            if ((opponentPosition > 77) || (opponentPosition < 6)) {
                opponentPosition = 40;
            }

            SisReceived = false;
        }
        opponent.draw(opponentPosition);

    }


    // shutdown the connection since we're done
    SiResult = shutdown(ClientSocket, SD_SEND);
    if (SiResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}

void Sreceive() {
    do {

        SiResult = recv(ClientSocket, Srecvbuf, Srecvbuflen, 0);
        if (SiResult > 0) {
            
            SisReceived = true;
            
        }
        else if (SiResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            break;
        }

    } while (SiResult > 0);
}
#include <cstring>
#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

// ws2_32.lib 를 링크한다.
#pragma comment(lib, "Ws2_32.lib")

static unsigned short SERVER_PORT = 27015;

int main()
{
    int r = 0;

    // Winsock 을 초기화한다.
    WSADATA wsaData;
    r = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (r != NO_ERROR) {
        std::cerr << "WSAStartup failed with error " << r << std::endl;
        return 1;
    }

    // UDP socket 을 만든다.
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed with error " << WSAGetLastError() << std::endl;
        return 1;
    }

    // socket 을 특정 주소, 포트에 바인딩 한다.
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    r = bind(sock, (sockaddr *) &serverAddr, sizeof(serverAddr));
    if (r != 0) {
        std::cerr << "bind failed with error " << WSAGetLastError() << std::endl;
        return 1;
    }

    // socket 으로부터 데이터를 받는다.
    // UDP 는 연결 기반이 아니므로 누가 보냈는지를 매 datagram 별로 입력 받는다.
    std:: cout << "Receiving datagrams" << std::endl;
    struct sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buf[65536 * 2];
    r = recvfrom(sock, buf, sizeof(buf), 0, (sockaddr *) &clientAddr, &clientAddrSize);
    if (r == SOCKET_ERROR) {
        std::cerr << "recvfrom failed with error " << WSAGetLastError() << std::endl;
        return 1;
    }

    std::cout << "Received " << r << " bytes" << std::endl;

    // Socket 을 닫는다.
    r = closesocket(sock);
    if (r == SOCKET_ERROR) {
        std::cerr << "closesocket failed with error " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Winsock 을 정리한다.
    WSACleanup();
    return 0;
}

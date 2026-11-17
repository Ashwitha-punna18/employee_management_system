#include <stdio.h>
#include <winsock2.h>

int main()
{
    WSADATA wsa;

    SOCKET client_socket;

    struct sockaddr_in server_address;

    char message[] = "Hello from Sender!";

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(client_socket,
            (struct sockaddr*)&server_address,
            sizeof(server_address));

    printf("Connected to Receiver!\n");

    // Send message
    send(client_socket, message, sizeof(message), 0);

    printf("Message sent successfully!\n");

    // Close socket
    closesocket(client_socket);

    WSACleanup();

    return 0;
}

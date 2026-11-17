#include <stdio.h>
#include <winsock2.h>

int main()
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    int client_size = sizeof(client_address);
    char buffer[1024];

    WSAStartup(MAKEWORD(2, 2), &wsa);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket,
         (struct sockaddr*)&server_address,
         sizeof(server_address));

    listen(server_socket, 1);

    printf("Receiver is waiting for sender...\n");

    client_socket = accept(server_socket,
                           (struct sockaddr*)&client_address,
                           &client_size);

    printf("Sender connected!\n");

    int bytes_received = recv(client_socket,
                              buffer,
                              sizeof(buffer) - 1,
                              0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
        printf("Message received: %s\n", buffer);
    }

    closesocket(client_socket);
    closesocket(server_socket);

    WSACleanup();

    return 0;
}
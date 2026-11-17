#include <stdio.h>
#include <winsock2.h>
#include <string.h>

int main()
{
    int receive_all(SOCKET socket, char *data, int length)
{
    int total_received = 0;

    while (total_received < length)
    {
        int received_now = recv(socket,
                                data + total_received,
                                length - total_received,
                                0);

        if (received_now <= 0)
        {
            return SOCKET_ERROR;
        }

        total_received += received_now;
    }

    return total_received;
}

    WSADATA wsa;
    SOCKET server_socket, client_socket;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    int client_size = sizeof(client_address);

    char buffer[1024];
    char filename[256];
    char filepath[512];

    unsigned long long file_size;
    unsigned long long remaining;

    int file_count;
    int i;
    int filename_length;
    int bytes_received;

    FILE *file;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
 {
    printf("Socket creation failed!\n");
    WSACleanup();
    return 1;
}

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
if (bind(server_socket,
         (struct sockaddr*)&server_address,
         sizeof(server_address)) == SOCKET_ERROR)
{
    printf("Binding failed!\n");
    closesocket(server_socket);
    WSACleanup();
    return 1;
}

    // Listen for sender
if (listen(server_socket, 1) == SOCKET_ERROR)
{
    printf("Listening failed!\n");
    closesocket(server_socket);
    WSACleanup();
    return 1;
}

    printf("Receiver is waiting for file...\n");

    // Accept sender connection
client_socket = accept(server_socket,
                       (struct sockaddr*)&client_address,
                       &client_size);   

if (client_socket == INVALID_SOCKET)
{
    printf("Accept failed!\n");
    closesocket(server_socket);
    WSACleanup();
    return 1;
}

if (receive_all(client_socket, (char*)&file_count, sizeof(file_count)) == SOCKET_ERROR)
{
    printf("Error receiving file count!\n");
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 1;
}

printf("Number of files to receive: %d\n", file_count);
if (file_count <= 0 || file_count > 100)
{
    printf("Invalid file count!\n");
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 1;
}

printf("Sender connected!\n");

    // Open destination file
   // Receive multiple files
for (i = 0; i < file_count; i++)
{
    // Receive filename length
    if (receive_all(client_socket, (char*)&filename_length,
                    sizeof(filename_length)) == SOCKET_ERROR)
    {
        printf("Error receiving filename length!\n");
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (filename_length <= 0 || filename_length >= sizeof(filename))
    {
        printf("Invalid filename length!\n");
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Receive filename
    if (receive_all(client_socket, filename, filename_length) == SOCKET_ERROR)
    {
        printf("Error receiving filename!\n");
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    filename[filename_length] = '\0';

    // Prevent directory paths in filenames
    
    if (strchr(filename, '/') != NULL ||
    strchr(filename, '\\') != NULL ||
    strcmp(filename, ".") == 0 ||
    strcmp(filename, "..") == 0)
    {
        printf("Invalid filename!\n");
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Receive file size
    if (receive_all(client_socket, (char*)&file_size,
                    sizeof(file_size)) == SOCKET_ERROR)
    {
        printf("Error receiving file size!\n");
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    snprintf(filepath, sizeof(filepath), "received_files/%s", filename);

    file = fopen(filepath, "wb");

    if (file == NULL)
    {
        printf("Error creating received file!\n");
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Receive file contents
    remaining = file_size;

    while (remaining > 0)
    {
        int chunk_size = remaining > sizeof(buffer)
                         ? sizeof(buffer) : (int)remaining;

        bytes_received = recv(client_socket, buffer, chunk_size, 0);

        if (bytes_received <= 0)
        {
            printf("Error receiving file data!\n");
            fclose(file);
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }

        if (fwrite(buffer, 1, bytes_received, file) != bytes_received)
        {
            printf("Error writing file!\n");
            fclose(file);
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }

        remaining -= bytes_received;
    }

    fclose(file);

    printf("File received: %s\n", filename);
}

printf("All files received successfully!\n");

   

    closesocket(client_socket);
    closesocket(server_socket);

    WSACleanup();

    return 0;
}
#include <stdio.h>
#include <winsock2.h>
#include <string.h>

int main()
{
    char *filenames[] = {
    "sender/sample.txt",
    "sender/notes.txt",
    "sender/document.txt"
};

int file_count = 3;
int i;
int filename_length;
unsigned long long file_size;

int send_all(SOCKET socket, const char *data, int length)
{
    int total_sent = 0;

    while (total_sent < length)
    {
        int sent_now = send(socket,
                            data + total_sent,
                            length - total_sent,
                            0);

        if (sent_now == SOCKET_ERROR || sent_now == 0)
        {
            return SOCKET_ERROR;
        }

        total_sent += sent_now;
    }

    return total_sent;
}
    WSADATA wsa;
    SOCKET client_socket;

    struct sockaddr_in server_address;

    char buffer[1024];
    int bytes_read;

    FILE *file;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure receiver address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to receiver
    connect(client_socket,
            (struct sockaddr*)&server_address,
            sizeof(server_address));

    printf("Connected to Receiver!\n");
    
if (send_all(client_socket, (char*)&file_count, sizeof(file_count)) == SOCKET_ERROR)
{
    printf("Error sending file count!\n");
    closesocket(client_socket);
    WSACleanup();
    return 1;
}

// Send multiple files
for (i = 0; i < file_count; i++)
{
    file = fopen(filenames[i], "rb");

    if (file == NULL)
    {
        printf("Error opening file!\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Get file name
    char *name = strrchr(filenames[i], '/');

    if (name != NULL)
        name++;
    else
        name = filenames[i];

    filename_length = strlen(name);

    // Send filename length
    if (send_all(client_socket, (char*)&filename_length,
             sizeof(filename_length)) == SOCKET_ERROR)
    {
    printf("Error sending filename length!\n");
    fclose(file);
    closesocket(client_socket);
    WSACleanup();
    return 1;
    }

// Send filename
if (send_all(client_socket, name, filename_length) == SOCKET_ERROR)
{
    printf("Error sending filename!\n");
    fclose(file);
    closesocket(client_socket);
    WSACleanup();
    return 1;
}
   

    // Get file size
    long file_size_temp;

if (fseek(file, 0, SEEK_END) != 0)
{
    printf("Error finding file size!\n");
    fclose(file);
    closesocket(client_socket);
    WSACleanup();
    return 1;
}

file_size_temp = ftell(file);

if (file_size_temp < 0)
{
    printf("Error reading file size!\n");
    fclose(file);
    closesocket(client_socket);
    WSACleanup();
    return 1;
}

file_size = (unsigned long long)file_size_temp;

if (fseek(file, 0, SEEK_SET) != 0)
{
    printf("Error returning to beginning of file!\n");
    fclose(file);
    closesocket(client_socket);
    WSACleanup();
    return 1;
}


    // Send file size
   if (send_all(client_socket, (char*)&file_size,
             sizeof(file_size)) == SOCKET_ERROR)
{
    printf("Error sending file size!\n");
    fclose(file);
    closesocket(client_socket);
    WSACleanup();
    return 1;
}
   
    // Send file contents
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (send_all(client_socket, buffer, bytes_read) == SOCKET_ERROR)
        {
            printf("Error sending file!\n");
            fclose(file);
            closesocket(client_socket);
            WSACleanup();
            return 1;
        }
    }
if (ferror(file))
{
    printf("Error reading file!\n");
    fclose(file);
    closesocket(client_socket);
    WSACleanup();
    return 1;
}

    fclose(file);

    printf("File sent: %s\n", name);
}

printf("All files sent successfully!\n");


    fclose(file);
    closesocket(client_socket);

    WSACleanup();

    return 0;
}
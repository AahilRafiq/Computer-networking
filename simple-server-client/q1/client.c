#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_port = htons(3001);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);

    int conn_result = connect(socket_fd, (struct sockaddr*)&address, sizeof(address));

    if(conn_result == 0) {
        printf("Connection Success\n");
    } else {
        printf("Failed Connection\n");
        close(socket_fd);
        return 1;
    }

    char message[1024*50];
    char buffer[1024*100];

    while(1) {
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;  // Remove trailing newline

        int sending_result = send(socket_fd, message, strlen(message), 0);

        if(sending_result == -1) {
            printf("Error sending data\n");
            break;
        }

        int recv_result = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        
        if(recv_result <= 0) {
            printf("Error receiving data or server disconnected\n");
            break;
        }

        printf("Server: %s\n", buffer);
    }

    close(socket_fd);

    return 0;
}
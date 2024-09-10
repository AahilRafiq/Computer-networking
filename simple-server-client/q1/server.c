#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(3001);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(socket_fd, (struct sockaddr*)&address, sizeof(address));

    listen(socket_fd, 5);

    socklen_t addrlen = sizeof(struct sockaddr_in);
    int client_fd = accept(socket_fd, (struct sockaddr*)&address, &addrlen);

    if(client_fd == -1) {
        printf("Error accepting client\n");
        close(socket_fd);
        return 1;
    }

    char buffer[1024*20];

    while(1) {
        int recv_result = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if(recv_result <= 0) {
            recv_result == 0 ? printf("Client Disconnected\n") : printf("Error receiving data \n");
            break;
        }

        printf("Client: %s\n", buffer);

        int send_res = send(client_fd,"ACK",strlen("ACK"),0);
    }

    close(client_fd);
    close(socket_fd);

    return 0;
}
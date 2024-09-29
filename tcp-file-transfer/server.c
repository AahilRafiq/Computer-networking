#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(3000);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(socket_fd, (struct sockaddr*)&address,sizeof(address));

    listen(socket_fd, 5);
    printf("Listening for Connections\n");

    socklen_t addrlen = sizeof(struct sockaddr_in);
    int client_fd = accept(socket_fd, (struct sockaddr*)&address,&addrlen);

    if(client_fd == -1) {
        printf("Error accepting client\n");
        close(socket_fd);
        return 1;
    }

    char file_name[1024];
    size_t bytes_recvd;
    void *buffer = malloc(1024);

    //get file name first
    int file_name_len;
    recv(client_fd, &file_name_len,sizeof(int),0);
    recv(client_fd, file_name,file_name_len,0);
    file_name[file_name_len] = '\0';
    printf("%s\n",file_name);

    char full_path[1024] = "downloads/";
    strcat(full_path,file_name);
    FILE *recv_file = fopen(full_path,"w");

    while((bytes_recvd = recv(client_fd,buffer,1024,0)) > 0) {
        fwrite(buffer,1,bytes_recvd,recv_file);
    }

    close(socket_fd);
    close(client_fd);
    return 0;
}
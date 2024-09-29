#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 1024

int send_file_name(int socket_fd, char *file_name) {
    // first sending file name length
    unsigned int file_name_len = strlen(file_name);
    send(socket_fd, &file_name_len, sizeof(int),0);
    
    // send actual file name
    send(socket_fd, file_name,file_name_len,0);
}

int main(int argc, char *argv[])
{
    char *file_name = argv[1];
    FILE *file = fopen(file_name,"r");
    if(file == NULL) {
        printf("Error reading file\n");
        return 1;
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(3000);
    inet_pton(AF_INET,"127.0.0.1",&address.sin_addr.s_addr);

    int conn_result = connect(socket_fd,(struct sockaddr*)&address,sizeof(address));

    if(conn_result != 0) {
        printf("Connection failed");
        close(socket_fd);
        return 1;
    }

    send_file_name(socket_fd, file_name);

    size_t bytes_read;
    void *buffer = malloc(BUFFER_SIZE);
    
    while((bytes_read = fread(buffer,sizeof(*buffer),BUFFER_SIZE,file)) > 0) {
        send(socket_fd, buffer,1024,0);
    }
    
    close(socket_fd);
    return 0;
}

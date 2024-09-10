#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
// #define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    while(1) {
        char weather_data[] = "Temperature: 25C, Humidity: 60%%, Wind: 10km/h";
        
        sendto(sockfd, (const char *)weather_data, strlen(weather_data), 
               MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        
        printf("Weather data sent.\n");
        sleep(5);  // Wait for 5 seconds 
    }
    
    close(sockfd);
    return 0;
}
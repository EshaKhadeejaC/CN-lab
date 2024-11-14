#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 100
#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_size = sizeof(clientAddr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for frames...\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addr_size);
        if (recv_len < 0) {
            perror("Receive failed");
            break;
        }

        buffer[recv_len] = '\0';
        printf("Received: %s\n", buffer);

        int frame_number;
        sscanf(buffer, "Frame %d", &frame_number);
        snprintf(buffer, BUFFER_SIZE, "Ack for frame %d", frame_number);

        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, addr_size) < 0) {
            perror("Acknowledgment send failed");
            break;
        }

        printf("Ack sent for frame %d\n", frame_number);
    }

    close(sockfd);
    return 0;
}

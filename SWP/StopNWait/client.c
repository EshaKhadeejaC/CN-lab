#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 100
#define PORT 8080
#define IP_ADDRESS "127.0.0.1"

int main() {
    int sockfd;
    struct sockaddr_in receiverAddr;
    char buffer[BUFFER_SIZE];
    int noframes, i = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT);
    receiverAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    printf("Enter the number of frames: ");
    scanf("%d", &noframes);

    while (noframes > 0) {
        snprintf(buffer, BUFFER_SIZE, "Frame %d", i);
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr)) < 0) {
            perror("Frame send failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Sent frame %d\n", i);

        memset(buffer, 0, BUFFER_SIZE);
        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (recv_len < 0) {
            perror("Acknowledgment receive failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        buffer[recv_len] = '\0';
        printf("Ack received: %s\n", buffer);

        noframes--;
        i++;
        sleep(1);
    }

    printf("End of Stop-and-Wait protocol\n");
    close(sockfd);
    return 0;
}

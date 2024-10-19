#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addrSize;
    fd_set readfds;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure settings of the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    addrSize = sizeof(serverAddr);

    // Connect the socket to the server
    connect(clientSocket, (struct sockaddr *)&serverAddr, addrSize);

    while (1) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add stdin and client socket to set
        FD_SET(0, &readfds);  // Standard input
        FD_SET(clientSocket, &readfds);  // Client socket

        // Wait for activity
        select(clientSocket + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(0, &readfds)) {
            // Send message
            printf("Enter the msg to send: ");
            fgets(buffer, sizeof(buffer), stdin);
            send(clientSocket, buffer, strlen(buffer), 0);
        }

        if (FD_ISSET(clientSocket, &readfds)) {
            // Receive broadcasted message
            int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
            buffer[valread] = '\0';
            printf("Broadcasted message: %s\n", buffer);
        }
    }

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_CLIENTS 10

int main(){
    int welcomeSocket, newSocket, clientSockets[MAX_CLIENTS];
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addrSize;
    int i, max_sd, sd, activity;
    fd_set readfds;

    // Initialize all clientSocket[] to 0 so that no client is being used
    for (i = 0; i < MAX_CLIENTS; i++) {
        clientSockets[i] = 0;
    }

    // Create socket
    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure settings of the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
    
    // Bind the socket
    bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    
    // Listen for incoming connections
    if (listen(welcomeSocket, 5) == 0) {
        printf("Listening..\n");
    } else {
        printf("Error..\n");
        exit(1);
    }

    addrSize = sizeof(serverAddr);

    while (1) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add welcomeSocket to set
        FD_SET(welcomeSocket, &readfds);
        max_sd = welcomeSocket;

        // Add child sockets to set
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = clientSockets[i];

            // If valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            // Highest file descriptor number for select()
            if (sd > max_sd)
                max_sd = sd;
        }

        // Wait for an activity on one of the sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(welcomeSocket, &readfds)) {
            // Accept new incoming connection
            newSocket = accept(welcomeSocket, (struct sockaddr *)&serverAddr, &addrSize);
            printf("New connection established\n");

            // Add new socket to array of clients
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (clientSockets[i] == 0) {
                    clientSockets[i] = newSocket;
                    break;
                }
            }
        }

        // IO operation on some other socket
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = clientSockets[i];

            if (FD_ISSET(sd, &readfds)) {
                // Check if it was for closing, and also read the incoming message
                int valread = recv(sd, buffer, 1024, 0);

                if (valread == 0) {
                    // Someone disconnected, get details and print
                    close(sd);
                    clientSockets[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    
                    // Broadcast the message to all other clients
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (clientSockets[j] != 0 && clientSockets[j] != sd) {
                            send(clientSockets[j], buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

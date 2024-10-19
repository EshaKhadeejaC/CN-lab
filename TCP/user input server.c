#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void main() 
{
    int server, newSock, port;
    char buffer[1024], ip_address[16], message[1024];
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;

    // Take user input for IP address and port number
    printf("Enter IP Address (e.g., 127.0.0.1): ");
    scanf("%s", ip_address);

    printf("Enter Port Number (e.g., 6265): ");
    scanf("%d", &port);

    // Create socket
    server = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address struct
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);  // Convert port to network byte order
    servAddr.sin_addr.s_addr = inet_addr(ip_address);  // Convert IP address to binary format

    // Bind the socket to the IP and port
    bind(server, (struct sockaddr *) &servAddr, sizeof(servAddr));

    // Listen for incoming connections
    if (listen(server, 5) == 0)
        printf("Listening on %s:%d\n", ip_address, port);
    else
        printf("Error\n");

    // Accept incoming connection
    addrSize = sizeof(store);
    newSock = accept(server, (struct sockaddr *) &store, &addrSize);

    // Enter the loop to continuously send messages
    while (1) {
        // Receive data from the client
        recv(newSock, buffer, 1024, 0);
        printf("2. Data received: %s\n", buffer);

        // Ask user for a message to send to the client
        printf("Enter Message to send to client (or type 'exit' to stop): ");
        scanf(" %[^\n]%*c", message); // Reads the entire line including spaces

        // Check if the user wants to exit
        if (strcmp(message, "exit") == 0) {
            printf("Exiting...\n");
            break;  // Exit the loop if the user types 'exit'
        }

        // Send the message to the client
        strcpy(buffer, message);
        printf("3. Sending data to client..\n");
        send(newSock, buffer, strlen(buffer), 0);
    }

    // Close the sockets after exiting the loop
    close(newSock);
    close(server);
}

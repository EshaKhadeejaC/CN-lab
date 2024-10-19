#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

void main()
{
    int client, port;
    char buffer[1024], ip_address[16], message[1024];
    struct sockaddr_in servAddr;
    socklen_t addrSize;

    // Take user input for IP address and port number
    printf("Enter Server IP Address (e.g., 127.0.0.1): ");
    scanf("%s", ip_address);

    printf("Enter Server Port Number (e.g., 6265): ");
    scanf("%d", &port);

    // Create socket
    client = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address struct
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);  // Convert port to network byte order
    servAddr.sin_addr.s_addr = inet_addr(ip_address);  // Convert IP address to binary format

    // Connect to the server
    connect(client, (struct sockaddr*)&servAddr, sizeof(servAddr));

    // Enter the loop to continuously send messages
    while (1) {
        // Ask user for a message to send to the server
        printf("Enter Message to send to server (or type 'exit' to stop): ");
        scanf(" %[^\n]%*c", message);  // Reads the entire line including spaces

        // Check if the user wants to exit
        if (strcmp(message, "exit") == 0) {
            printf("Exiting...\n");
            break;  // Exit the loop if the user types 'exit'
        }

        // Send the message to the server
        printf("1. Sending data to server...\n");
        strcpy(buffer, message);
        send(client, buffer, strlen(buffer), 0);

        // Receive response from the server
        recv(client, buffer, 1024, 0);
        printf("4. Data received from server: %s\n", buffer);
    }

    // Close the socket after exiting the loop
    close(client);
}

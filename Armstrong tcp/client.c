#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Enter a number: ");
    int number;
    scanf("%d", &number);
    snprintf(buffer, sizeof(buffer), "%d", number);

    printf("Sending number to server: %d\n", number);
    write(client_socket, buffer, strlen(buffer) + 1);

    read(client_socket, buffer, sizeof(buffer));
    printf("Received from server: %s\n", buffer);

    close(client_socket);
    return 0;
}

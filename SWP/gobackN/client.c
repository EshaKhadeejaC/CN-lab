#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9009
#define SERVER_IP "127.0.0.1"

int main() {
    int client_sock;
    struct sockaddr_in server_addr;
    char buffer[100];
    char ack_msg[50];

    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Connection failed\n");
        return 0;
    }

    printf("\nClient - with individual acknowledgement scheme\n\n");

    for (int i = 0; i < 10; i++) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(client_sock, buffer, sizeof(buffer));

        if (buffer[strlen(buffer) - 1] == i + '0') {
            printf("Message received from server: %s\n", buffer);
            snprintf(ack_msg, sizeof(ack_msg), "acknowledgement of : %d", i);
            printf("Acknowledgement sent for message\n");
            write(client_sock, ack_msg, strlen(ack_msg) + 1);
        } else {
            printf("Discarded as out of order\n");
            i--;
        }
    }

    close(client_sock);
    return 0;
}

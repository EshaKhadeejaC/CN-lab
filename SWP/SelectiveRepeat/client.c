#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int is_faulty() {
    return (rand() % 4) > 2;  // 25% chance of corruption
}

int main() {
    srand(time(0));
    int c_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client = {0};
    client.sin_family = AF_INET;
    client.sin_port = htons(9009);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1) {
        printf("Connection failed\n");
        return 1;
    }

    printf("Client Connected\n");
    char buff[100];
    int msg_count = 0;

    while (msg_count < 9) {
        read(c_sock, buff, sizeof(buff));
        printf("Received: %s\n", buff);

        char response[50];
        int corrupted = is_faulty();
        if (corrupted) {
            sprintf(response, "negative ack %c", buff[strlen(buff) - 1]);
        } else {
            sprintf(response, "acknowledgment %c", buff[strlen(buff) - 1]);
            msg_count++;
        }
        printf("Sending: %s\n", response);
        write(c_sock, response, sizeof(response));
    }

    close(c_sock);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

void resend(int msg_num, int c_sock) {
    char buff[60];
    sprintf(buff, "resend message: %d", msg_num);
    printf("Resending: %s\n", buff);
    write(c_sock, buff, sizeof(buff));
    usleep(1000);
}

int main() {
    int s_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server = {0}, client = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(9009);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s_sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("Binding failed\n");
        return 1;
    }

    printf("Server Ready\n");
    listen(s_sock, 10);

    socklen_t client_len = sizeof(client);
    int c_sock = accept(s_sock, (struct sockaddr *)&client, &client_len);

    int total_msgs = 0;
    while (total_msgs < 9) {
        // Send three messages in sequence
        for (int i = 0; i < 3; ++i) {
            char buff[50];
            sprintf(buff, "server message: %d", total_msgs + i);
            printf("Sent: %s\n", buff);
            write(c_sock, buff, sizeof(buff));
            usleep(1000);
        }

        // Await acknowledgments
        for (int i = 0; i < 3; ++i) {
            fd_set set;
            FD_ZERO(&set);
            FD_SET(c_sock, &set);
            struct timeval timeout = {2, 0};
            int rv = select(c_sock + 1, &set, NULL, NULL, &timeout);

            if (rv == 0) {
                printf("Timeout: Message %d\n", total_msgs + i);
                resend(total_msgs + i, c_sock);
                i--;  // Retry the same message
            } else if (rv > 0) {
                char buff[50];
                read(c_sock, buff, sizeof(buff));
                if (buff[0] == 'n') {  // If negative acknowledgment
                    printf("Corrupted ack: Message %d\n", buff[strlen(buff) - 1] - '0');
                    resend(buff[strlen(buff) - 1] - '0', c_sock);
                    i--;  // Retry the same message
                } else {
                    total_msgs++;
                }
            }
        }
    }
    close(c_sock);
    close(s_sock);
    return 0;
}

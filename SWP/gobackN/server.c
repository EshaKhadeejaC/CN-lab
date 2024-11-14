#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 9009
#define TIMEOUT_SEC 2
#define TOTAL_MSGS 10
#define GO_BACK_N 3

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    fd_set read_fds;
    struct timeval timeout;
    char msg[50];
    char buffer[50];
    int msg_num = 0;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("\tServer Up\nGo back n (n=3) used to send 10 messages\n\n");

    listen(server_sock, 10);
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);

    while (msg_num < TOTAL_MSGS) {
        snprintf(msg, sizeof(msg), "server message: %d", msg_num);
        printf("Message sent to client: %s\n", msg);
        send(client_sock, msg, strlen(msg) + 1, 0);

        FD_ZERO(&read_fds);
        FD_SET(client_sock, &read_fds);
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;

        if (select(client_sock + 1, &read_fds, NULL, NULL, &timeout) > 0) {
            recv(client_sock, buffer, sizeof(buffer), 0);
            printf("Message from Client: %s\n", buffer);
            msg_num++;
        } else {
            printf("Timeout! Going back from %d\n", msg_num);
            msg_num = (msg_num - GO_BACK_N >= 0) ? msg_num - GO_BACK_N : 0;
        }
    }

    close(client_sock);
    close(server_sock);
    return 0;
}


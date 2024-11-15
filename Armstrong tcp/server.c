#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>

int isArmstrong(int number) {
    int original = number, sum = 0, n = 0, remainder;
    
    // Count digits
    int temp = number;
    while (temp != 0) {
        temp /= 10;
        n++;
    }
    
    // Calculate Armstrong sum
    temp = number;
    while (temp != 0) {
        remainder = temp % 10;
        sum += pow(remainder, n);
        temp /= 10;
    }
    
    return sum == original;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    printf("Server is listening on port 8080...\n");

    addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);

    read(client_socket, buffer, sizeof(buffer));
    int number = atoi(buffer);
    printf("Received number from client: %d\n", number);

    int is_armstrong = isArmstrong(number);
    snprintf(buffer, sizeof(buffer), "Number %d is %s", number, is_armstrong ? "an Armstrong number" : "not an Armstrong number");

    printf("Result sent to client: %s\n", buffer);
    write(client_socket, buffer, strlen(buffer) + 1);

    close(client_socket);
    close(server_socket);
    return 0;
}

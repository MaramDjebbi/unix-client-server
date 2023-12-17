#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define NMAX 100 

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Generate random number
    srand(time(NULL));
    int n = rand() % NMAX + 1;

    // Send the random number to server
    if (sendto(sockfd, &n, sizeof(n), 0, (const struct sockaddr *)&server_addr, addr_len) < 0) {
        perror("Sending failed");
        exit(EXIT_FAILURE);
    }

    // Receive and display response from server
    int receivedNums[NMAX];
    if (recvfrom(sockfd, receivedNums, sizeof(receivedNums), 0, (struct sockaddr *)&server_addr, &addr_len) < 0) {
        perror("Receiving failed");
        exit(EXIT_FAILURE);
    }

    printf("Received numbers from server:\n");
    for (int i = 0; i < n; i++) {
        printf("%d\n", receivedNums[i]);
    }

    close(sockfd);
    return 0;
}


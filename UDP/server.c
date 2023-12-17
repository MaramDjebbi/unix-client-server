#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define NMAX 100 

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is running on port %s...\n", argv[1]);

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // Bind socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    int receivedNum;
    if (recvfrom(sockfd, &receivedNum, sizeof(receivedNum), 0, (struct sockaddr *)&client_addr, &addr_len) < 0) {
        perror("Receiving failed");
        exit(EXIT_FAILURE);
    }

    // Generate 'n' random numbers
    int randomNumbers[NMAX];
    srand(time(NULL));
    for (int i = 0; i < receivedNum; i++) {
        randomNumbers[i] = rand();
    }

    // Send the 'n' random numbers to client
    if (sendto(sockfd, randomNumbers, sizeof(randomNumbers), 0, (struct sockaddr *)&client_addr, addr_len) < 0) {
        perror("Sending failed");
        exit(EXIT_FAILURE);
    }

    close(sockfd);
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>

#define MAXLINE 4096
#define DEFAULT_PORT 3000
#define TIMEOUT 5

void printUsage(const char *progname) {
    fprintf(stderr, "Usage: %s <IP address> <port>\n", progname);
}

int createNewSocket(){
    int sockfd;
    // SOCK_STREAM for TCP
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.\n");
    return sockfd;
}

void setTimeOut(int sockfd){
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("Error setting socket timeout");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Socket timeout set successfully.\n");
}

int main(int argc, char **argv) {
    int sockfd, port;
    struct sockaddr_in servaddr;
    char buf[MAXLINE];

    // Check the arguments
    if (argc < 2 || argc > 3) {
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Set the port number
    port = (argc == 3) ? atoi(argv[2]) : DEFAULT_PORT;

    sockfd = createNewSocket();

    // Set a timeout for the socket operations
    setTimeOut(sockfd);

    // Initialize the server address structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // Convert ipv4
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    // Convert port number
    servaddr.sin_port = htons(port); // convert to big-endian order

    // Connect the client to the server
    printf("Attempting to connect to the server at %s:%d...\n", argv[1], port);

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Problem in connecting to the server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server successfully.\n");

    // Send message to server
    char message[MAXLINE];
    while(1){
        printf("Send a message: ");
        fgets(message, MAXLINE, stdin);
    
        if (send(sockfd, message, strlen(message), 0) < 0) {
            perror("Error sending message");
            break;
        }
        // Listen for a message from the server
        int n = recv(sockfd, buf, MAXLINE, 0);
        if (n < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
            printf("No message received within the timeout period.\n");
            } else {
            perror("Error receiving message");
            break;
            }
        } else if (n == 0) {
            printf("Server closed the connection.\n");
            break;
        } else {
            buf[n] = '\0'; // Null-terminate the received data
            printf("Server: %s\n", buf);
        }
    }

    close(sockfd);
    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define MAXLINE 4096    /* max text line length */
#define SERV_PORT 3000  /* port */
#define LISTENQ 8       /* maximum number of client connections */
#define MAXCLIENTS 100  /* maximum number of clients */

int client_sockets[MAXCLIENTS];
int client_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    // Create a socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem creating socket");
        exit(1);
    }

    // Prepare the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    // Bind the socket
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Bind error");
        exit(1);
    }

    // Listen on the socket
    if (listen(listenfd, LISTENQ) < 0)
    {
        perror("Listen error");
        exit(1);
    }

    printf("Server running...waiting for connections.\n");

    while (1)
    {
        clilen = sizeof(cliaddr);
        // Accept a connection
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
        {
            perror("Accept error");
            continue;
        }

        printf("Received request...\n");

        pthread_t tid;

        // Add client socket to the list
        pthread_mutex_lock(&mutex);
        client_sockets[client_count++] = connfd;
        pthread_mutex_unlock(&mutex);

        // Create a thread to handle the client
        if (pthread_create(&tid, NULL, &handle_client, (void *)&connfd) != 0)
        {
            perror("Thread creation error");
            continue;
        }
    }

    return 0;
}

void *handle_client(void *arg)
{
    int connfd = *(int *)arg;
    char buf[MAXLINE];
    int n;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Get the client's address
    getpeername(connfd, (struct sockaddr *)&addr, &addr_len);
    char client_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, client_address, INET_ADDRSTRLEN);

    printf("Child thread created for client requests\n");

    while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
    {
        buf[n] = '\0';
        printf("Message received from client %s: %s", client_address, buf);

        // Prepend the sender's address to the message
        char message_with_sender[MAXLINE + INET_ADDRSTRLEN + 2];
        snprintf(message_with_sender, sizeof(message_with_sender), "%s: %s", client_address, buf);

        // Broadcast the message to all other clients
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < client_count; i++)
        {
            if (client_sockets[i] != connfd)
            {
                send(client_sockets[i], message_with_sender, strlen(message_with_sender), 0);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    if (n == 0)
    {
        printf("Client %s disconnected.\n", client_address);
    }
    else if (n < 0)
    {
        perror("Read error");
    }

    // Remove the client socket from the list
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < client_count; i++)
    {
        if (client_sockets[i] == connfd)
        {
            for (int j = i; j < client_count - 1; j++)
            {
                client_sockets[j] = client_sockets[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);

    close(connfd);
    pthread_exit(NULL);
}
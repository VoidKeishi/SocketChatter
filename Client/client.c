#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAXLINE 4096
#define DEFAULT_PORT 3000

int sockfd;
GtkWidget *text_view;
GtkWidget *entry;
GtkTextTag *left_tag;
char client_address[INET_ADDRSTRLEN];

void printUsage(const char *progname) {
    fprintf(stderr, "Usage: %s <IP address> <port>\n", progname);
}

int createNewSocket() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully.\n");
    return sockfd;
}

void *receive_messages(void *arg) {
    char buf[MAXLINE];
    int n;
    GtkTextBuffer *buffer;
    GtkTextIter end;

    while ((n = recv(sockfd, buf, MAXLINE, 0)) > 0) {
        buf[n] = '\0';
        gdk_threads_enter();
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_get_end_iter(buffer, &end);

        // Check if the message is from the client itself
        if (strncmp(buf, client_address, strlen(client_address)) == 0) {
            gtk_text_buffer_insert_with_tags(buffer, &end, "You: ", -1, left_tag, NULL);
            gtk_text_buffer_insert(buffer, &end, buf + strlen(client_address) + 2, -1);
        } else {
            gtk_text_buffer_insert_with_tags(buffer, &end, buf, -1, left_tag, NULL);
        }
        gtk_text_buffer_insert(buffer, &end, "\n", -1);
        gdk_threads_leave();
    }

    if (n == 0) {
        printf("Server closed the connection.\n");
    } else if (n < 0) {
        perror("Error receiving message");
    }

    close(sockfd);
    return NULL;
}

void send_message(GtkWidget *widget, gpointer data) {
    const gchar *message = gtk_entry_get_text(GTK_ENTRY(entry));
    if (send(sockfd, message, strlen(message), 0) < 0) {
        perror("Error sending message");
    }

    // Display the sent message in the text view
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert_with_tags(buffer, &end, "You: ", -1, left_tag, NULL);
    gtk_text_buffer_insert(buffer, &end, message, -1);
    gtk_text_buffer_insert(buffer, &end, "\n", -1);

    gtk_entry_set_text(GTK_ENTRY(entry), "");
}

int main(int argc, char **argv) {
    struct sockaddr_in servaddr;
    pthread_t recv_thread;

    if (argc < 2 || argc > 3) {
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = (argc == 3) ? atoi(argv[2]) : DEFAULT_PORT;

    sockfd = createNewSocket();

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    servaddr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Problem in connecting to the server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server successfully.\n");

    // Get the client's address
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    getsockname(sockfd, (struct sockaddr *)&addr, &addr_len);
    inet_ntop(AF_INET, &addr.sin_addr, client_address, INET_ADDRSTRLEN);

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Client");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 0);

    // Create a text tag for left alignment
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);
    left_tag = gtk_text_tag_new("left_align");
    g_object_set(left_tag, "justification", GTK_JUSTIFY_LEFT, NULL);
    gtk_text_tag_table_add(tag_table, left_tag);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

    GtkWidget *send_button = gtk_button_new_with_label("Send");
    gtk_box_pack_start(GTK_BOX(vbox), send_button, FALSE, FALSE, 0);
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), NULL);

    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    gtk_widget_show_all(window);
    gtk_main();

    pthread_join(recv_thread, NULL);
    close(sockfd);

    return 0;
}
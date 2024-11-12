#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAXLINE 4096
#define DEFAULT_PORT 4000

int sockfd;
GtkWidget *login_window;
GtkWidget *message_window;
GtkWidget *text_view;
GtkWidget *entry;
GtkWidget *username_entry;
GtkWidget *password_entry;
GtkTextTag *left_tag;
char client_address[INET_ADDRSTRLEN];
char username[50];

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
        char *colon_pos = strchr(buf, ':');
        if (colon_pos != NULL) {
            *colon_pos = '\0';
            const char *sender = buf;
            const char *message = colon_pos + 2;

            if (strcmp(sender, client_address) == 0) {
                gtk_text_buffer_insert_with_tags(buffer, &end, "You: ", -1, left_tag, NULL);
            } else {
                gtk_text_buffer_insert_with_tags(buffer, &end, sender, -1, left_tag, NULL);
                gtk_text_buffer_insert(buffer, &end, ": ", -1);
            }
            gtk_text_buffer_insert(buffer, &end, message, -1);
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

void authenticate(GtkWidget *widget, gpointer data) {
    const gchar *username_text = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password_text = gtk_entry_get_text(GTK_ENTRY(password_entry));
    char buf[MAXLINE];

    snprintf(buf, sizeof(buf), "LOGIN %s %s", username_text, password_text);
    if (send(sockfd, buf, strlen(buf), 0) < 0) {
        perror("Error sending login message");
        return;
    }

    int n = recv(sockfd, buf, MAXLINE, 0);
    if (n <= 0) {
        perror("Error receiving login response");
        return;
    }
    buf[n] = '\0';

    if (strcmp(buf, "LOGIN_SUCCESS\n") == 0) {
        strcpy(username, username_text);
        gtk_widget_hide(login_window);
        gtk_widget_show_all(message_window);
    } else {
        printf("Login failed.\n");
    }
}

void register_user(GtkWidget *widget, gpointer data) {
    const gchar *username_text = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password_text = gtk_entry_get_text(GTK_ENTRY(password_entry));
    char buf[MAXLINE];

    snprintf(buf, sizeof(buf), "REGISTER %s %s", username_text, password_text);
    if (send(sockfd, buf, strlen(buf), 0) < 0) {
        perror("Error sending register message");
        return;
    }

    int n = recv(sockfd, buf, MAXLINE, 0);
    if (n <= 0) {
        perror("Error receiving register response");
        return;
    }
    buf[n] = '\0';

    if (strcmp(buf, "REGISTER_SUCCESS\n") == 0) {
        printf("Registration successful. Please log in.\n");
    } else {
        printf("Registration failed.\n");
    }
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

    // Create login window
    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(login_window), "Login/Register");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 400, 200);
    g_signal_connect(login_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *login_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(login_window), login_vbox);

    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Username");
    gtk_box_pack_start(GTK_BOX(login_vbox), username_entry, FALSE, FALSE, 0);

    password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(login_vbox), password_entry, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    gtk_box_pack_start(GTK_BOX(login_vbox), login_button, FALSE, FALSE, 0);
    g_signal_connect(login_button, "clicked", G_CALLBACK(authenticate), NULL);

    GtkWidget *register_button = gtk_button_new_with_label("Register");
    gtk_box_pack_start(GTK_BOX(login_vbox), register_button, FALSE, FALSE, 0);
    g_signal_connect(register_button, "clicked", G_CALLBACK(register_user), NULL);

    // Create message window
    message_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(message_window), "Chat Client");
    gtk_window_set_default_size(GTK_WINDOW(message_window), 400, 300);
    g_signal_connect(message_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *message_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(message_window), message_vbox);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_box_pack_start(GTK_BOX(message_vbox), text_view, TRUE, TRUE, 0);

    // Create a text tag for left alignment
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);
    left_tag = gtk_text_tag_new("left_align");
    g_object_set(left_tag, "justification", GTK_JUSTIFY_LEFT, NULL);
    gtk_text_tag_table_add(tag_table, left_tag);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(message_vbox), entry, FALSE, FALSE, 0);

    GtkWidget *send_button = gtk_button_new_with_label("Send");
    gtk_box_pack_start(GTK_BOX(message_vbox), send_button, FALSE, FALSE, 0);
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), NULL);

    gtk_widget_show_all(login_window);
    gtk_widget_hide(message_window);

    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    gtk_main();

    pthread_join(recv_thread, NULL);
    close(sockfd);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void display_chat(const char *message) {
    printf("%s\n", message);
}

void handle_monitor(int sock, const char *username, const char *channel_name, const char *room_name) {
    fd_set readfds;
    struct timeval timeout;
    int max_sd, activity, sock_fd = sock;
    char buffer[BUFFER_SIZE];

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(sock_fd, &readfds);
        max_sd = sock_fd;

        timeout.tv_sec = 5; // Set timeout to 5 seconds
        timeout.tv_usec = 0;

        activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);

        if ((activity < 0) && (errno != EINTR)) {
            printf("Select error\n");
        }

        if (FD_ISSET(sock_fd, &readfds)) {
            int bytes_received = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                display_chat(buffer);
            } else if (bytes_received == 0) {
                printf("Server disconnected\n");
                break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5 || strcmp(argv[1], "-channel") != 0 || strcmp(argv[3], "-room") != 0) {
        printf("Usage:\n");
        printf("./monitor -channel channel_name -room room_name\n");
        return 1;
    }

    char *username = "Monitor"; // Set default username for monitor
    char *channel_name = argv[2];
    char *room_name = argv[4];

    int sock;
    struct sockaddr_in server_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    char command[BUFFER_SIZE];
    sprintf(command, "LOGIN %s -p password", username);
    send(sock, command, strlen(command), 0);

    sprintf(command, "JOIN %s", channel_name);
    send(sock, command, strlen(command), 0);

    sprintf(command, "JOIN %s/%s", channel_name, room_name);
    send(sock, command, strlen(command), 0);

    handle_monitor(sock, username, channel_name, room_name);

    close(sock);
    return 0;
}

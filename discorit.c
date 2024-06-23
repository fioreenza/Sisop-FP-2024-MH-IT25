#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_command(int sock, const char *command) {
    char buffer[BUFFER_SIZE];

    send(sock, command, strlen(command), 0);
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("%s\n", buffer);
    }
}

void handle_commands(int sock, const char *username) {
    char command[BUFFER_SIZE];
    char currentChannel[BUFFER_SIZE] = "";
    char currentRoom[BUFFER_SIZE] = "";
    int loggedIn = 1;

    while (loggedIn) {
        if (currentChannel[0] != '\0' && currentRoom[0] != '\0') {
            printf("[%s/%s/%s] ", username, currentChannel, currentRoom);
        } else if (currentChannel[0] != '\0') {
            printf("[%s/%s] ", username, currentChannel);
        } else {
            printf("[%s] ", username);
        }

        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        send(sock, command, strlen(command), 0);

        char buffer[BUFFER_SIZE];
        int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';

            if (strstr(buffer, "berhasil login")) {
                printf("[%s] %s\n", username, buffer);
            } else if (strstr(buffer, "bergabung ke channel ")) {
                char *channel_name = strstr(buffer, "bergabung ke channel ");
                if (channel_name != NULL) {
                    channel_name += strlen("bergabung ke channel ");
                    sscanf(channel_name, "%s", currentChannel);
                    currentRoom[0] = '\0'; // Clear the room when joining a new channel
                    printf("[%s/%s] %s\n", username, currentChannel, buffer);
                } else {
                    printf("%s\n", buffer); // Default print if channel name not found
                }
            } else if (strstr(buffer, "bergabung ke room ")) {
                char *room_name = strstr(buffer, "bergabung ke room ");
                if (room_name != NULL) {
                    room_name += strlen("room dibuat ");
                    sscanf(room_name, "%s", currentRoom);
                    printf("[%s/%s/%s] %s\n", username, currentChannel, currentRoom, buffer);
                } else {
                    printf("%s\n", buffer); // Default print if room name not found
                }
            } else {
                printf("%s\n", buffer);
            }
        }
        if (strcmp(command, "LOGOUT") == 0) {
            loggedIn = 0;
        } else if (strcmp(command, "LEAVE CHANNEL") == 0) {
            currentChannel[0] = '\0';
            currentRoom[0] = '\0';
        } else if (strcmp(command, "LEAVE ROOM") == 0) {
            currentRoom[0] = '\0';
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5 || (strcmp(argv[1], "REGISTER") != 0 && strcmp(argv[1], "LOGIN") != 0)) {
        printf("Usage:\n");
        printf("./discorit REGISTER username -p password\n");
        printf("./discorit LOGIN username -p password\n");
        return 1;
    }

    char username[BUFFER_SIZE];
    strcpy(username, argv[2]);

    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

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
    sprintf(command, "%s %s -p %s", argv[1], argv[2], argv[4]);

    send_command(sock, command);

    if (strcmp(argv[1], "LOGIN") == 0) {
        handle_commands(sock, username);
    }

    close(sock);
    return 0;
}

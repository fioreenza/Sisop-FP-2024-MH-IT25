#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int loggedIn = 0;
char currentChannel[BUFFER_SIZE] = "";
char username[BUFFER_SIZE] = ""; 

void send_command(int sock, const char *command) {
    char buffer[BUFFER_SIZE];

    send(sock, command, strlen(command), 0);
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';

        if (loggedIn && strncmp(command, "JOIN CHANNEL", 12) == 0) {
            char command_copy[BUFFER_SIZE];
            strcpy(command_copy, command);
            char *channelName = strtok(command_copy + 13, " ");
            if (channelName != NULL) {
                strcpy(currentChannel, channelName);
            }
        }

        if (loggedIn && currentChannel[0] != '\0') {
            printf("[%s/%s] %s\n", username, currentChannel, buffer); 
        } else if (loggedIn) {
            printf("[%s] %s\n", username, buffer); 
        } else {
            printf("%s\n", buffer); 
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

    strcpy(username, argv[2]); // Copy username from argv[2] to global variable

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
        loggedIn = 1;
        while (1) {
            if (loggedIn && currentChannel[0] != '\0') {
                printf("[%s/%s] ", username, currentChannel);
            } else if (loggedIn) {
                printf("[%s] ", username); 
            } else {
                puts(""); 
            }
            
            fgets(command, sizeof(command), stdin);
            command[strcspn(command, "\n")] = '\0'; 

            send_command(sock, command);

            if (strncmp(command, "JOIN CHANNEL", 12) == 0) {
                char command_copy[BUFFER_SIZE];
                strcpy(command_copy, command);
                char *channelName = strtok(command_copy + 13, " ");
                if (channelName != NULL) {
                    strcpy(currentChannel, channelName);
                }
            } else if (strcmp(command, "LOGOUT") == 0) {
                break;
            }
        }
    }

    close(sock);
    return 0;
}

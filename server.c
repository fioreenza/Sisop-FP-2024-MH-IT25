#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <bcrypt.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_USERS 100
#define BUFFER_SIZE 1024
#define MAX_ROOMS 100
#define MAX_ROOM_NAME_LEN 50
#define CSV_FILE "DiscorIT/users.csv"
#define CHANNELS_FILE "DiscorIT/channels.csv"
#define MAX_LINE_LENGTH 1024

typedef struct {
    int id_user;
    char name[50];
    char password[100];
    char role[10]; //root atau user
} User;

typedef struct {
    int id_channel;
    char name[50];
    char key[BCRYPT_HASHSIZE];
} Channel;

typedef struct {
    int id_user;
    char name[50];
    char role[10]; // untuk mengetahui admin SAJA
} AuthEntry;


typedef struct {
    char name[MAX_ROOM_NAME_LEN];
} Room;

typedef struct {
    bool is_authenticated;
    bool in_channel;
    bool in_room;
    char username[50];
    char current_channel[50];
    char current_room[50];
} ClientState;



Room rooms[MAX_ROOMS];
int num_rooms = 0;

bool file_exists(const char *filename) {
    return access(filename, F_OK) != -1;
}

void initialize_csv() {
    // Check if DiscorIT directory exists
    if (access("DiscorIT", F_OK) == -1) {
        // Directory does not exist, create it
        if (mkdir("DiscorIT", 0777) == -1) {
            perror("Failed to create DiscorIT directory");
            exit(EXIT_FAILURE);
        }
    }

    if (!file_exists(CSV_FILE)) {
        FILE *file = fopen(CSV_FILE, "w");
        if (file) {
            fprintf(file, "id_user,name,password,role\n");
            fclose(file);
        } else {
            perror("Failed to initialize users.csv");
            exit(EXIT_FAILURE);
        }
    }
}


bool read_csv(User users[], int *count) {
    FILE *file = fopen(CSV_FILE, "r");
    if (!file) return false;

    char line[MAX_LINE_LENGTH];
    *count = 0;

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        User user;
        sscanf(line, "%d,%49[^,],%99[^,],%9s", &user.id_user, user.name, user.password, user.role);
        users[(*count)++] = user;
    }

    fclose(file);
    return true;
}

bool write_csv(User users[], int count) {
    FILE *file = fopen(CSV_FILE, "w");
    if (!file) return false;

    fprintf(file, "id_user,name,password,role\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%s\n", users[i].id_user, users[i].name, users[i].password, users[i].role);
    }

    fclose(file);
    return true;
}

bool initialize_channels_csv() {
    if (!file_exists(CHANNELS_FILE)) {
        FILE *file = fopen(CHANNELS_FILE, "w");
        if (file) {
            fprintf(file, "id_channel,name,key\n");
            fclose(file);
        } else {
            perror("Failed to initialize channels.csv");
            return false;
        }
    }
    return true;
}

bool read_channels(Channel channels[], int *count) {
    FILE *file = fopen(CHANNELS_FILE, "r");
    if (!file) return false;

    char line[MAX_LINE_LENGTH];
    *count = 0;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Channel channel;
        sscanf(line, "%d,%49[^,],%49s", &channel.id_channel, channel.name, channel.key);
        channels[(*count)++] = channel;
    }

    fclose(file);
    return true;
}

bool write_channels(Channel channels[], int count) {
    FILE *file = fopen(CHANNELS_FILE, "w");
    if (!file) return false;

    fprintf(file, "id_channel,name,key\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s\n", channels[i].id_channel, channels[i].name, channels[i].key);
    }

    fclose(file);
    return true;
}

bool bcrypt_hash_channel_key(const char *key, char *hash) {
    char salt[BCRYPT_HASHSIZE];
    if (bcrypt_gensalt(12, salt) != 0) {
        return false;
    }
    return bcrypt_hashpw(key, salt, hash) == 0;
}

bool bcrypt_hash(const char *password, char *hash) {
    char salt[BCRYPT_HASHSIZE];
    if (bcrypt_gensalt(12, salt) != 0) {
        return false;
    }
    return bcrypt_hashpw(password, salt, hash) == 0;
}

bool bcrypt_check(const char *password, const char *hash) {
    return bcrypt_checkpw(password, hash) == 0;
}


bool initialize_auth_csv(const char *channel_name) {
    char filename[100];
    snprintf(filename, sizeof(filename), "DiscorIT/%s/admin/auth.csv", channel_name);

    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }

    file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "id_user,name,role\n");
        fclose(file);
        return true;
    } else {
        perror("Failed to initialize auth.csv");
        return false;
    }
}


bool read_auth(const char *channel_name, AuthEntry entries[], int *count) {
    char filename[100];
    snprintf(filename, sizeof(filename), "DiscorIT/%s/admin/auth.csv", channel_name);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open auth.csv for reading");
        return false;
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        AuthEntry entry;
        if (sscanf(line, "%d,%49[^,],%9s", &entry.id_user, entry.name, entry.role) == 3) {
            entries[(*count)++] = entry;
        } else {
            fprintf(stderr, "Error parsing line: %s", line);
        }
    }

    fclose(file);
    return true;
}

bool read_auth_confirm(const char *channel_name, AuthEntry entries[], int *count) {
    char filename[100];
    snprintf(filename, sizeof(filename), "DiscorIT/%s/admin/auth.csv", channel_name);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open auth.csv for reading");
        return false;
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;

    while (fgets(line, sizeof(line), file)) {
        AuthEntry entry;
        if (sscanf(line, "%d,%49[^,],%9s", &entry.id_user, entry.name, entry.role) == 3) {
            entries[(*count)++] = entry;
        } else {
            fprintf(stderr, "Error parsing line: %s", line);
        }
    }

    fclose(file);
    return true;
}

bool write_auth(const char *channel_name, AuthEntry entries[], int *count) {
    char filename[100];
    snprintf(filename, sizeof(filename), "DiscorIT/%s/admin/auth.csv", channel_name);

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open auth.csv for writing");
        return false;
    }

    for (int i = 0; i < *count; i++) {
        fprintf(file, "%d,%s,%s\n", entries[i].id_user, entries[i].name, entries[i].role);
    }

    fclose(file);
    
    return true;
}

void register_user(int client_sock, const char *username, const char *password) {
    User users[100];
    int count = 0;

    if (file_exists(CSV_FILE)) {
        read_csv(users, &count);

        for (int i = 0; i < count; i++) {
            if (strcmp(users[i].name, username) == 0) {
                char response[100];
                sprintf(response, "%s sudah terdaftar\n", username);
                send(client_sock, response, strlen(response), 0);
                return;
            }
        }
    }

    User new_user;
    new_user.id_user = count + 1;
    strcpy(new_user.name, username);
    if (!bcrypt_hash(password, new_user.password)) {
        char response[] = "Error hashing password\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }
    strcpy(new_user.role, count == 0 ? "ROOT" : "USER");

    users[count++] = new_user;
    write_csv(users, count);

    char response[100];
    sprintf(response, "%s berhasil register\n", username);
    send(client_sock, response, strlen(response), 0);
}

void list_users(int client_sock) {
    User users[100];
    int count = 0;

    if (!read_csv(users, &count)) {
        char response[] = "Error reading user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[BUFFER_SIZE] = "";
    for (int i = 0; i < count; i++) {
        strcat(response, users[i].name);
        strcat(response, " ");
    }
    strcat(response, "\n");
    send(client_sock, response, strlen(response), 0);
}

void edit_user(int client_sock, const char *old_username, const char *new_username) {
    User users[100];
    int count = 0;

    if (!read_csv(users, &count)) {
        char response[] = "Error reading user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].name, old_username) == 0) {
            strcpy(users[i].name, new_username);
            write_csv(users, count);
            char response[100];
            sprintf(response, "%s berhasil diubah menjadi %s\n", old_username, new_username);
            send(client_sock, response, strlen(response), 0);
            return;
        }
    }

    char response[100];
    sprintf(response, "%s tidak ditemukan\n", old_username);
    send(client_sock, response, strlen(response), 0);
}

void edit_password(int client_sock, const char *username, const char *new_password) {
    User users[100];
    int count = 0;

    if (!read_csv(users, &count)) {
        char response[] = "Error reading user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].name, username) == 0) {
            if (!bcrypt_hash(new_password, users[i].password)) {
                char response[] = "Error hashing password\n";
                send(client_sock, response, strlen(response), 0);
                return;
            }
            write_csv(users, count);
            char response[100];
            sprintf(response, "Password untuk %s berhasil diubah\n", username);
            send(client_sock, response, strlen(response), 0);
            return;
        }
    }

    char response[100];
    sprintf(response, "%s tidak ditemukan\n", username);
    send(client_sock, response, strlen(response), 0);
}

void remove_user(int client_sock, const char *username) {
    User users[100];
    int count = 0;

    if (!read_csv(users, &count)) {
        char response[] = "Error reading user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].name, username) == 0) {
            for (int j = i; j < count - 1; j++) {
                users[j] = users[j + 1];
            }
            count--;
            write_csv(users, count);
            char response[100];
            sprintf(response, "%s berhasil dihapus\n", username);
            send(client_sock, response, strlen(response), 0);
            return;
        }
    }

    char response[100];
    sprintf(response, "%s tidak ditemukan\n", username);
    send(client_sock, response, strlen(response), 0);
}

bool is_root_user(const char* username) {
    User users[100];
    int user_count = 0;

    if (!read_csv(users, &user_count)) {
        return false; // If reading the CSV fails, we cannot determine the user's role
    }

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, username) == 0 && strcmp(users[i].role, "ROOT") == 0) {
            return true;
        }
    }

    return false;
}

void create_channel(int client_sock, const char *username, const char *channel_name, const char *key) {
    Channel channels[100];
    int count = 0;

    if (!initialize_channels_csv() || !read_channels(channels, &count)) {
        char response[] = "Error reading channel data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Check if channel_name already exists
    for (int i = 0; i < count; i++) {
        if (strcmp(channels[i].name, channel_name) == 0) {
            char response[100];
            sprintf(response, "Channel %s already exists\n", channel_name);
            send(client_sock, response, strlen(response), 0);
            return;
        }
    }

    // Add new channel
    Channel new_channel;
    new_channel.id_channel = count + 1;
    strcpy(new_channel.name, channel_name);
    // Encrypt key using bcrypt (just an example)
    if (!bcrypt_hash(key, new_channel.key)) {
        char response[] = "Error hashing channel key\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    channels[count++] = new_channel;
    write_channels(channels, count);
    
    // Create directory for the channel
    char channel_directory[100];
    sprintf(channel_directory, "DiscorIT/%s", channel_name);
    if (mkdir(channel_directory, 0777) == -1) {
        char response[] = "Error creating channel directory\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Create admin directory inside the channel directory
    char admin_directory[100];
    sprintf(admin_directory, "%s/admin", channel_directory);
    if (mkdir(admin_directory, 0777) == -1) {
        char response[] = "Error creating admin directory\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }
    
    
  // Initialize auth.csv and add admin entry

    int auth_count = 0;
    AuthEntry entries[10];
    AuthEntry admin_entry;
    admin_entry.id_user = 1;
    strcpy(admin_entry.name, username);
    if (is_root_user(username)) {
        strcpy(admin_entry.role, "ROOT");
    } else {
        strcpy(admin_entry.role, "ADMIN");
    }

    if (!initialize_auth_csv(channel_name) || !read_auth(channel_name, entries, &auth_count)) {
        char response[] = "Error reading channel data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    entries[auth_count++] = admin_entry;

    if (!write_auth(channel_name, entries, &auth_count)) {
        char response[] = "Error writing auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }


    char response[100];
    sprintf(response, "Channel %s dibuat\n", channel_name);
    send(client_sock, response, strlen(response), 0);
}



void edit_channel_name(int client_sock, const char *old_channel_name, const char *new_channel_name, const char *username) {
    Channel channels[100];
    int count = 0;

    if (!read_channels(channels, &count)) {
        char response[] = "Error reading channel data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool channel_found = false;
    int channel_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(channels[i].name, old_channel_name) == 0) {
            channel_found = true;
            channel_index = i;
            break;
        }
    }

    if (!channel_found) {
        char response[100];
        sprintf(response, "Channel %s tidak ditemukan\n", old_channel_name);
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Check if the user is an admin in the specified channel
    char auth_path[100];
    snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", old_channel_name);

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(old_channel_name, entries, &auth_count)) {
        char response[] = "Error reading auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool is_admin = false;
    for (int i = 0; i < auth_count; i++) {
        if (strcmp(entries[i].name, username) == 0 && strcmp(entries[i].role, "ADMIN") == 0) {
            is_admin = true;
            break;
        }
    }

    // Check if the user is ROOT
    User users[100];
    int user_count = 0;

    if (!read_csv(users, &user_count)) {
        char response[] = "Error reading user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool is_root = false;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, username) == 0 && strcmp(users[i].role, "ROOT") == 0) {
            is_root = true;
            break;
        }
    }

    if (!is_admin && !is_root) {
        char response[] = "Anda tidak memiliki izin untuk mengedit nama channel\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Update channel name
    strcpy(channels[channel_index].name, new_channel_name);
    if (!write_channels(channels, count)) {
        char response[] = "Error updating channel data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Update directory name
    char old_directory[100];
    char new_directory[100];
    sprintf(old_directory, "DiscorIT/%s", old_channel_name);
    sprintf(new_directory, "DiscorIT/%s", new_channel_name);

    if (rename(old_directory, new_directory) != 0) {
        char response[] = "Error renaming channel directory\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[100];
    sprintf(response, "Channel %s berhasil diubah menjadi %s\n", old_channel_name, new_channel_name);
    send(client_sock, response, strlen(response), 0);
}

int remove_directory(const char *path) {
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d) {
        struct dirent *p;
        r = 0;
        while (!r && (p = readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf) {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        r2 = remove_directory(buf);
                    } else {
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }

    if (!r) {
        r = rmdir(path);
    }

    return r;
}


void delete_channel(int client_sock, const char *channel_name, const char *username) {
    Channel channels[100];
    int count = 0;

    if (!read_channels(channels, &count)) {
        char response[] = "Error reading channel data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool channel_found = false;
    int channel_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(channels[i].name, channel_name) == 0) {
            channel_found = true;
            channel_index = i;
            break;
        }
    }

    if (!channel_found) {
        char response[100];
        sprintf(response, "Channel %s tidak ditemukan\n", channel_name);
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Check if the user is an admin in the specified channel
    char auth_path[100];
    snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", channel_name);

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(channel_name, entries, &auth_count)) {
        char response[] = "Error reading auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool is_admin = false;
    for (int i = 0; i < auth_count; i++) {
        if (strcmp(entries[i].name, username) == 0 && strcmp(entries[i].role, "ADMIN") == 0) {
            is_admin = true;
            break;
        }
    }

    // Check if the user is ROOT
    User users[100];
    int user_count = 0;

    if (!read_csv(users, &user_count)) {
        char response[] = "Error reading user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool is_root = false;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, username) == 0 && strcmp(users[i].role, "ROOT") == 0) {
            is_root = true;
            break;
        }
    }

    if (!is_admin && !is_root) {
        char response[] = "Anda tidak memiliki izin untuk menghapus channel\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Remove the channel from channels.csv
    for (int i = channel_index; i < count - 1; i++) {
        channels[i] = channels[i + 1];
    }
    count--;

    if (!write_channels(channels, count)) {
        char response[] = "Error updating channel data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Delete channel directory recursively
    char channel_directory[100];
    sprintf(channel_directory, "DiscorIT/%s", channel_name);
    if (remove_directory(channel_directory) == -1) {
        char response[] = "Error deleting channel directory\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[100];
    sprintf(response, "Channel %s berhasil dihapus\n", channel_name);
    send(client_sock, response, strlen(response), 0);
}



void list_channels(int client_sock) {
    Channel channels[100];
    int count = 0;

    if (!read_channels(channels, &count)) {
        char response[] = "Error reading channel data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }
    
    char response[BUFFER_SIZE] = "";
        
    for (int i = 0; i < count; i++) {
        strcat(response, channels[i].name);
        strcat(response, " ");
    }
    strcat(response, "\n");
    send(client_sock, response, strlen(response), 0);
}

void join_channel(int client_sock, ClientState *state, const char *username, const char *channel_name) {
    Channel channels[100];
    int count = 0;

    if (!read_channels(channels, &count)) {
        char response[] = "Error reading channel data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool channel_found = false;
    for (int i = 0; i < count; i++) {
        if (strcmp(channels[i].name, channel_name) == 0) {
            channel_found = true;
            break;
        }
    }

    if (!channel_found) {
        char response[100];
        sprintf(response, "Channel %s tidak ditemukan\n", channel_name);
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[100];
    sprintf(response, "%s bergabung ke channel %s", username, channel_name);
    send(client_sock, response, strlen(response), 0);

    // Update client state to reflect that the user is in a channel
    state->in_channel = true;
    strcpy(state->current_channel, channel_name);
}


void create_room(int client_sock, ClientState *state, const char *room_name) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before creating a room.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char path[100];
    snprintf(path, sizeof(path), "DiscorIT/%s/%s", state->current_channel, room_name);

    if (mkdir(path, 0777) == -1) {
        char response[100];
        sprintf(response, "Failed to create room: %s\n", strerror(errno));
        send(client_sock, response, strlen(response), 0);
    } else {
        char response[100];
        sprintf(response, "Room %s created in channel %s\n", room_name, state->current_channel);
        send(client_sock, response, strlen(response), 0);
    }
}


void rename_room(int client_sock, ClientState *state, const char *old_room_name, const char *new_room_name) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before renaming a room.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char old_path[200], new_path[200];
    snprintf(old_path, sizeof(old_path), "DiscorIT/%s/%s", state->current_channel, old_room_name);
    snprintf(new_path, sizeof(new_path), "DiscorIT/%s/%s", state->current_channel, new_room_name);

    if (rename(old_path, new_path) == -1) {
        char response[] = "Error renaming room. Make sure the room exists and the new name is unique.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[100];
    snprintf(response, sizeof(response), "Room %s renamed to %s in channel %s.\n", old_room_name, new_room_name, state->current_channel);
    send(client_sock, response, strlen(response), 0);
}

void delete_room(int client_sock, ClientState *state, const char *room_name) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before deleting a room.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char path[200];
    snprintf(path, sizeof(path), "DiscorIT/%s/%s", state->current_channel, room_name);

    if (rmdir(path) == -1) {
        char response[] = "Error deleting room. Make sure the room exists and is empty.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[100];
    snprintf(response, sizeof(response), "Room %s deleted from channel %s.\n", room_name, state->current_channel);
    send(client_sock, response, strlen(response), 0);
}

void join_room(int client_sock, ClientState *state, const char *room_name) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before joining a room.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char path[100];
    snprintf(path, sizeof(path), "DiscorIT/%s/%s", state->current_channel, room_name);

    struct stat st;
    if (stat(path, &st) == -1 || !S_ISDIR(st.st_mode)) {
        char response[100];
        sprintf(response, "Room %s does not exist in channel %s\n", room_name, state->current_channel);
        send(client_sock, response, strlen(response), 0);
    } else {
        state->in_room = true;
        strcpy(state->current_room, room_name);
        char response[100];
        sprintf(response, "You have joined the room %s in channel %s\n", room_name, state->current_channel);
        send(client_sock, response, strlen(response), 0);
    }
    
    char response[100];
    sprintf(response, "bergabung ke room %s", room_name);
    send(client_sock, response, strlen(response), 0);

}



void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    int bytes_received;
    ClientState state = {0};

    while ((bytes_received = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_received] = '\0';

        char command[BUFFER_SIZE], username[50], password[50];
        sscanf(buffer, "%s %s -p %s", command, username, password);

        if (strcmp(command, "REGISTER") == 0) {
            register_user(client_sock, username, password);
        } else if (strcmp(command, "LOGIN") == 0) {
            User users[100];
            int count = 0;

            if (!read_csv(users, &count)) {
                char response[] = "Error reading user data\n";
                send(client_sock, response, strlen(response), 0);
                continue;
            }

            bool user_found = false;
            bool is_root = false;
            for (int i = 0; i < count; i++) {
                if (strcmp(users[i].name, username) == 0) {
                    user_found = true;
                    is_root = strcmp(users[i].role, "ROOT") == 0;
                    if (bcrypt_check(password, users[i].password)) {
                        state.is_authenticated = true;
                        strcpy(state.username, username);
                        char response[100];
                        sprintf(response, "%s berhasil login\n", username);
                        send(client_sock, response, strlen(response), 0);

                        while (1) {
                            bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
                            if (bytes_received <= 0) {
                                break;
                            }
                            buffer[bytes_received] = '\0';

                            if (strcmp(buffer, "LIST USER") == 0) {
                                if (is_root) {
                                    list_users(client_sock);
                                } else {
                                    char response[] = "Anda tidak memiliki izin untuk melihat daftar pengguna\n";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } else if (strncmp(buffer, "EDIT WHERE ", 11) == 0) {
                                if (is_root) {
                                    char option[10], old_username[50], new_value[50];
                                    sscanf(buffer, "EDIT WHERE %s -%s %s", old_username, option, new_value);
                                    if (strcmp(option, "u") == 0) {
                                        edit_user(client_sock, old_username, new_value);
                                    } else if (strcmp(option, "p") == 0) {
                                        edit_password(client_sock, old_username, new_value);
                                    } else {
                                        char response[] = "Invalid EDIT command\n";
                                        send(client_sock, response, strlen(response), 0);
                                    }
                                } else {
                                    char response[] = "Anda tidak memiliki izin untuk mengedit pengguna\n";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } else if (strncmp(buffer, "REMOVE ", 7) == 0) {
                                if (is_root) {
                                    char username_to_remove[50];
                                    sscanf(buffer, "REMOVE %s", username_to_remove);
                                    remove_user(client_sock, username_to_remove);
                                } else {
                                    char response[] = "Anda tidak memiliki izin untuk menghapus pengguna\n";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } else if (strncmp(buffer, "CREATE CHANNEL ", 15) == 0) {
                                char option[10], channel_name[50], key[50];
                                sscanf(buffer, "CREATE CHANNEL %s -%s %s", channel_name, option, key);
                                if (strcmp(option, "k") == 0) {
                                    create_channel(client_sock, username, channel_name, key);
                                } else {
                                    char response[] = "Invalid CREATE command\n";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } else if (strncmp(buffer, "EDIT CHANNEL ", 13) == 0) {
                                char old_channel_name[50], new_channel_name[50];
                                int scanned = sscanf(buffer, "EDIT CHANNEL %s TO %s", old_channel_name, new_channel_name);
                                if (scanned == 2) {
                                    edit_channel_name(client_sock, old_channel_name, new_channel_name, username);
                                } else {
                                    char response[] = "Invalid EDIT CHANNEL command\n";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } else if (strncmp(buffer, "DEL CHANNEL ", 11) == 0) {
                                char channel_name[50];
                                sscanf(buffer, "DEL CHANNEL %s", channel_name);
                                delete_channel(client_sock, channel_name, username);
                            } else if (strcmp(buffer, "LIST CHANNEL") == 0) {
                                list_channels(client_sock);
                            } else if (strncmp(buffer, "JOIN CHANNEL ", 13) == 0) {
                                char channel_name[50];
                                sscanf(buffer, "JOIN CHANNEL %s", channel_name);
                                join_channel(client_sock, &state, username, channel_name);
                            } else if (strncmp(buffer, "CREATE ROOM ", 12) == 0) {
                                char room_name[50];
                                sscanf(buffer, "CREATE ROOM %s", room_name);
                                create_room(client_sock, &state, room_name);
                            } else if (strncmp(buffer, "EDIT ROOM ", 12) == 0) {
                                char old_room_name[50], new_room_name[50];
                                sscanf(buffer, "EDIT ROOM %s TO %s", old_room_name, new_room_name);
                                rename_room(client_sock, &state, old_room_name, new_room_name);
                            } else if (strncmp(buffer, "DEL ROOM ", 12) == 0) {
                                char room_name[50];
                                sscanf(buffer, "DEL ROOM %s", room_name);
                                delete_room(client_sock, &state, room_name);
                            } else if (strncmp(buffer, "JOIN ROOM ", 10) == 0) {
                                char room_name[50];
                                sscanf(buffer, "JOIN ROOM %s", room_name);
                                join_room(client_sock, &state, room_name);
                            } else if (strcmp(buffer, "LOGOUT") == 0) {
                                state.is_authenticated = false;
                                state.in_channel = false;
                                char response[100];
                                sprintf(response, "%s berhasil logout\n", username);
                                send(client_sock, response, strlen(response), 0);
                                break;
                            } else {
                                char response[] = "Command not recognized\n";
                                send(client_sock, response, strlen(response), 0);
                            }
                        }
                    } else {
                        char response[] = "Password salah\n";
                        send(client_sock, response, strlen(response), 0);
                        break;
                    }
                    break;
                }
            }

            if (!user_found) {
                char response[] = "Username tidak ditemukan\n";
                send(client_sock, response, strlen(response), 0);
            }
        }
    }
    close(client_sock);
}



void *client_handler(void *arg) {
    int client_sock = *(int *)arg;
    handle_client(client_sock);
    free(arg);
    pthread_exit(NULL);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t tid;

    initialize_csv();

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server berjalan di port %d\n", PORT);

    while (1) {
        if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
            perror("Accept failed");
            continue;
        }

        int *client_sock_ptr = malloc(sizeof(int));
        *client_sock_ptr = client_sock;

        if (pthread_create(&tid, NULL, client_handler, client_sock_ptr) != 0) {
            perror("Thread creation failed");
            close(client_sock);
            free(client_sock_ptr);
        }
    }

    close(server_sock);
    return 0;
}

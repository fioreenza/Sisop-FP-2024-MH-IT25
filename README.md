# Laporan Resmi Final-Project Kelompok IT25

**Anggota kelompok IT25**:
| Nama      | NRP         |
  |-----------|-------------|
  | Fiorenza Adelia Nalle   | 5027231053  |
  | Tio Axellino Irin  | 5027231065  |  
  | Wira Samudra Siregar  | 5027231041  |

# Deskripsi Final Project (Discorit)

## DiscorIT

Disclaimer
* Program server, discorit, dan monitor TIDAK DIPERBOLEHKAN menggunakan perintah system();

Bagaimana Program Diakses
* Untuk mengakses DiscorIT, user perlu membuka program client (discorit). discorit hanya bekerja sebagai client yang mengirimkan request user kepada server.

* Program server berjalan sebagai server yang menerima semua request dari client dan mengembalikan response kepada client sesuai ketentuan pada soal. Program server berjalan sebagai daemon. 

* Untuk hanya menampilkan chat, user perlu membuka program client (monitor). Lebih lengkapnya pada poin monitor.
* Program client dan server berinteraksi melalui socket.
* Server dapat terhubung dengan lebih dari satu client.
#### Tree
```
DiscorIT/
      - channels.csv
      - users.csv
      - channel1/
               - admin/
                        - auth.csv
                        - user.log
               - room1/
                        - chat.csv
               - room2/
                        - chat.csv
               - room3/
                        - chat.csv
      - channel2/
               - admin/
                        - auth.csv
                        - user.log
               - room1/
                        - chat.csv
               - room2/
                        - chat.csv
               - room3/
                        - chat.csv
```


# Keterangan setiap file
```
DiscorIT
users.csv
id_user	int (mulai dari 1)
name		string
password	string (di encrypt menggunakan bcrypt biar ga tembus)
global_role	string (pilihannya: ROOT / USER)

channels.csv
id_channel	int  (mulai dari 1)
channel	string
key		string (di encrypt menggunakan bcrypt biar ga tembus)
Channels
auth.csv
id_user	int
name		string
role		string (pilihannya: ROOT/ADMIN/USER/BANNED) 

user.log
[dd/mm/yyyy HH:MM:SS] admin buat room1
[dd/mm/yyyy HH:MM:SS] user1 masuk ke channel “say hi”
[dd/mm/yyyy HH:MM:SS] admin memberi role1 kepada user1
[dd/mm/yyyy HH:MM:SS] admin ban user1
Rooms
chat.csv
date		int
id_chat	number  (mulai dari 1)
sender 	string
chat		string
```

A. Autentikasi
* Setiap user harus memiliki username dan password untuk mengakses DiscorIT. Username, password, dan global role disimpan dalam file user.csv.
* Jika tidak ada user lain dalam sistem, user pertama yang mendaftar otomatis mendapatkan role "ROOT". Username harus bersifat unique dan password wajib di encrypt menggunakan menggunakan bcrypt.

1. Register
##### Format
```
./discorit REGISTER username -p password
username berhasil register
./discorit REGISTER same_username -p password
username sudah terdaftar
```

##### Contoh
```
./discorit REGISTER qurbancare -p qurban123
qurbancare berhasil register
./discorit REGISTER qurbancare -p qurban123
qurbancare sudah terdaftar
```
2. Login
##### Format
```
./discorit LOGIN username -p password
username berhasil login
[username] (bisa memberi input setelah tanda kurung)
```

##### Contoh
```
./discorit LOGIN qurbancare -p qurban123 
qurbancare berhasil login
[qurbancare] 
```
B. Bagaimana DiscorIT digunakan
**Perbedaan Akses antara Root, Admin, dan User**
* **Root**: Memiliki akses penuh untuk mengelola semua channel, room, dan user. Root adalah akun yang pertama kali mendaftar.
* **Admin**: Memiliki akses untuk mengelola channel dan room yang mereka buat, serta mengelola user dalam channel mereka.
* **User**: Dapat mengirim pesan chat, melihat channel, dan room. user menjadi admin di channel yang mereka buat.

1. List Channel dan Room
Setelah Login user dapat melihat daftar channel yang tersedia.
##### Format
```
[user] LIST CHANNEL
channel1 channel2 channel3
```

##### Contoh
```
[qurbancare] LIST CHANNEL
care bancar qurb
```
Setelah Masuk Channel user dapat melihat list room dan pengguna dalam channel tersebut
##### Format
```
[user/channel] LIST ROOM
room1 room2 room3
[user/channel] LIST USER
user1 user2 user3
```

##### Contoh
```
[qurbancare/care] LIST ROOM
urban banru runab
[qurbancare/care] LIST USER
root admin qurbancare
```
2. Akses Channel dan Room
Akses channel admin dan root
##### Format
```
[user] JOIN channel
[user/channel] 
```

##### Contoh
```
[qurbancare] JOIN care
[qurbancare/care] 
```
Ketika user pertama kali masuk ke channel, mereka memiliki akses terbatas. Jika mereka telah masuk sebelumnya, akses mereka meningkat sesuai dengan admin dan root.
##### Format
```
[user] JOIN channel
Key: key
[user/channel] 
```

##### Contoh
```
[qurbancare] JOIN care
Key: care123
[qurbancare/care] 
```
User dapat masuk ke room setelah bergabung dengan channel.
##### Format
```
[user/channel] JOIN room
[user/channel/room] 
```

##### Contoh
```
[qurbancare/care] JOIN urban
[qurbancare/care/urban] 
```
3. Fitur Chat
Setiap user dapat mengirim pesan dalam chat. ID pesan chat dimulai dari 1 dan semua pesan disimpan dalam file chat.csv. User dapat melihat pesan-pesan chat yang ada dalam room. Serta user dapat edit dan delete pesan yang sudah dikirim dengan menggunakan ID pesan.

##### Format
```
[user/channel/room] CHAT "text"
[user/channel/room] SEE CHAT
sebelumnya
[dd/mm/yyyy HH:MM:SS][id][username] “text”
sesudahnya
[user/channel/room] EDIT CHAT id “text”
[user/channel/room] DEL CHAT id 
```

##### Contoh
```
[qurbancare/care/urban] CHAT “hallo”
[qurbancare/care/urban] SEE CHAT
sebelumnya
[05/06/2024 23:22:12][3][qurbancare] “hallo”
sesudahnya
[qurbancare/care/urban] EDIT CHAT 3 “hi”
[qurbancare/care/urban] DEL CHAT 3
```
C. Root
* Akun yang pertama kali mendaftar otomatis mendapatkan peran "root".
* Root dapat masuk ke channel manapun tanpa key dan create, update, dan delete pada channel dan room, mirip dengan admin [D].
* Root memiliki kemampuan khusus untuk mengelola user, seperti: list, edit, dan Remove.
##### Format
```
[user] LIST USER
user1 user2 user3
[user] EDIT WHERE user1 -u user01
user1 berhasil diubah menjadi user01
[user] EDIT WHERE user01 -p secretpass
password user01 berhasil diubah
[user] REMOVE user01
user01 berhasil dihapus
```

##### Contoh
```
[root] LIST USER
naupan qurbancare bashmi
[root] EDIT WHERE naupan -u zika
naupan berhasil diubah menjadi zika
[root] EDIT WHERE zika -p 123zika
password zika berhasil diubah
[root] REMOVE zika
zika berhasil dihapus
```
D. Admin Channel
* Setiap user yang membuat channel otomatis menjadi admin di channel tersebut. Informasi tentang user disimpan dalam file auth.csv.
* Admin dapat create, update, dan delete pada channel dan room, serta dapat remove, ban, dan unban user di channel mereka.

1. Channel
Informasi tentang semua channel disimpan dalam file channel.csv. Semua perubahan dan aktivitas user pada channel dicatat dalam file users.log.
##### Format
```
[user] CREATE CHANNEL channel -k key
Channel channel dibuat
[user] EDIT CHANNEL old_channel TO new_channel
old_channel berhasil diubah menjadi new_channel
[user] DEL CHANNEL channel
channel berhasil dihapus
```

##### Contoh
```
[qurbancare] CREATE CHANNEL care -k care123
Channel care dibuat
[qurbancare] EDIT CHANNEL care TO cera
care berhasil diubah menjadi cera
[qurbancare] DEL CHANNEL cera
cera berhasil dihapus
```
2. Room
Semua perubahan dan aktivitas user pada room dicatat dalam file users.log.
##### Format
```
[user/channel] CREATE ROOM room 
Room room dibuat
[user/channel] EDIT ROOM old_room TO new_room
old_room berhasil diubah menjadi new_room
[user/channel] DEL ROOM room
room berhasil dihapus
[user/channel] DEL ROOM ALL
Semua room dihapus
```
##### Contoh
```
[qurbancare/care] CREATE ROOM urban
Room urban dibuat
[qurbancare/care] EDIT ROOM urban TO nabru
urban berhasil diubah menjadi nabru
[qurbancare/care] DEL ROOM nabru
nabru berhasil dihapus
[qurbancare/care] DEL ROOM ALL
Semua room dihapus
```
3. Ban 
Admin dapat melakukan ban pada user yang nakal. Aktivitas ban tercatat pada users.log. Ketika di ban, role "user" berubah menjadi "banned". Data tetap tersimpan dan user tidak dapat masuk ke dalam channel.
##### Format
```
[user/channel] BAN user1 
user1 diban
```

##### Contoh
```
[qurbancare/care] BAN pen
pen diban
```
4. Unban 
Admin dapat melakukan unban pada user yang sudah berperilaku baik. Aktivitas unban tercatat pada users.log. Ketika di unban, role "banned" berubah kembali menjadi "user" dan dapat masuk ke dalam channel.
##### Format
```
[user/channel] UNBAN user1 
user1 kembali
```

##### Contoh
```
[qurbancare/care] UNBAN pen
pen kembali
```
5. Remove user
Admin dapat remove user dan tercatat pada users.log.
##### Format
```
[user/channel] REMOVE USER user1 
user1 dikick
```

##### Contoh
```
[qurbancare/care] REMOVE USER pen
pen dikick
```

E. User

User dapat mengubah informasi profil mereka, user yang di ban tidak dapat masuk kedalam channel dan dapat keluar dari room, channel, atau keluar sepenuhnya dari DiscorIT.

1. Edit User Username
##### Format
```
[user] EDIT PROFILE SELF -u new_username
Profil diupdate
[new_username]
```

##### Contoh
```
[qurbancare] EDIT PROFILE SELF -u pen
Profil diupdate
[pen]
```

	NB: ketika username diubah maka username pada auth.csv ikut berubah. Untuk log dan chat username tidak berubah karena biarkan yang lalu tetap berlalu :)

2. Edit User Password
##### Format
```
[user] EDIT PROFILE SELF -p new_password
Password diupdate
```

##### Contoh
```
[qurbancare] EDIT PROFILE SELF -p pen123
Password diupdate
```
3. Banned user
##### Format
```
[user] JOIN channel
anda telah diban, silahkan menghubungi admin
```

##### Contoh
```
[qurbancare] JOIN care
anda telah diban, silahkan menghubungi admin
```
4. Exit
##### Format
```
[user/channel/room] EXIT
[user/channel] EXIT
[user] EXIT
```

##### Contoh
```
[qurbancare/care/urban] EXIT
[qurbancare/care] EXIT
[qurbancare] EXIT
```
F. Error Handling

Jika ada command yang tidak sesuai penggunaannya. Maka akan mengeluarkan pesan error dan tanpa keluar dari program client.

G. Monitor
* User dapat menampilkan isi chat secara real-time menggunakan program monitor. Jika ada perubahan pada isi chat, perubahan tersebut akan langsung ditampilkan di terminal.
* Sebelum dapat menggunakan monitor, pengguna harus login terlebih dahulu dengan cara yang mirip seperti login di DiscorIT.
* Untuk keluar dari room dan menghentikan program monitor dengan perintah "EXIT".
* Monitor dapat digunakan untuk menampilkan semua chat pada room, mulai dari chat pertama hingga chat yang akan datang nantinya.


##### Format
```
[username] -channel channel_name -room room_name
~isi chat~
sebelumnya
[dd/mm/yyyy HH:MM:SS][id][username] “text”
sesudahnya
[user/channel/room] EXIT
[user] EXIT
```

##### Contoh
```
[qurbancare] -channel care -room urban
~isi chat~
sebelumnya
[05/06/2024 23:22:12][3][qurbancare] “hallo”
sesudahnya
[qurbancare/care/urban] EXIT
[qurbancare] EXIT
```

Changelog
```
[13/06/2024] menambahkan penjelasan pada bagian Bagaimana Program Diakses
```

# Penyelesaian Discorit (Sebelum Revisi) 

## discorit.c
```c    
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

void handle_commands(int sock, char *username) {
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
                    room_name += strlen("bergabung ke room ");
                    sscanf(room_name, "%s", currentRoom);
                    printf("[%s/%s/%s] %s\n", username, currentChannel, currentRoom, buffer);
                } else {
                    printf("%s\n", buffer); // Default print if room name not found
                }
            } else if (strstr(buffer, "Profil diupdate")) {
                // Extract new_username from the command entered
                char new_username[50];
                sscanf(command, "EDIT PROFILE SELF -u %s", new_username);
                strncpy(username, new_username, BUFFER_SIZE - 1); // Update the username
                username[BUFFER_SIZE - 1] = '\0'; // Ensure null-terminated
                printf("Profil diupdate\n");
            } else if (strstr(buffer, "Bergabung ke room ")) {
                char *room_name = strstr(buffer, "Bergabung ke room ");
                if (room_name != NULL) {
                    room_name += strlen("Bergabung ke room ");
                    sscanf(room_name, "%s", currentRoom);
                    printf("[%s/%s/%s] %s\n", username, currentChannel, currentRoom, buffer);
                } else {
                    printf("%s\n", buffer); // Default print if room name not found
                }
            } else {
                printf("%s\n", buffer);
            }
        }

        if (strstr(buffer, "berhasil logout")) {
            loggedIn = 0;
        } else if (strstr(buffer, "berhasil keluar dari channel")) {
            currentChannel[0] = '\0';
        } else if (strstr(buffer, "berhasil keluar dari room")) {
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
```

## server.c

```c
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

void log_user_activity(const char *channel_name, const char *username, const char *activity) {
    char log_path[256];
    snprintf(log_path, sizeof(log_path), "DiscorIT/%s/admin/users.log", channel_name);

    // Get current time
    time_t now;
    struct tm *timestamp;
    char timestamp_str[20];
    time(&now);
    timestamp = localtime(&now);
    strftime(timestamp_str, sizeof(timestamp_str), "%d/%m/%Y %H:%M:%S", timestamp);

    // Open log file in append mode
    FILE *file = fopen(log_path, "a");
    if (file == NULL) {
        printf("Error opening log file for writing.\n");
        return;
    }

    // Write log entry
    fprintf(file, "[%s] %s %s\n", timestamp_str, username, activity);

    fclose(file);
}


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

    bool user_found = false;
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].name, old_username) == 0) {
            strcpy(users[i].name, new_username);
            user_found = true;
            break;
        }
    }

    if (user_found) {
        if (!write_csv(users, count)) {
            char response[] = "Error writing user data\n";
            send(client_sock, response, strlen(response), 0);
            return;
        }

        // Update username in auth.csv for all channels
        Channel channels[100];
        int channel_count = 0;

        if (!read_channels(channels, &channel_count)) {
            char response[] = "Error reading channel data\n";
            send(client_sock, response, strlen(response), 0);
            return;
        }

        for (int i = 0; i < channel_count; i++) {
            char auth_path[100];
            snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", channels[i].name);

            AuthEntry entries[10];
            int auth_count = 0;

            if (!read_auth_confirm(channels[i].name, entries, &auth_count)) {
                continue; // Skip to next channel if auth.csv reading fails
            }

            // Update username in auth.csv entries
            for (int j = 0; j < auth_count; j++) {
                if (strcmp(entries[j].name, old_username) == 0) {
                    strcpy(entries[j].name, new_username);
                }
            }

            // Write back updated auth.csv
            if (!write_auth(channels[i].name, entries, &auth_count)) {
                char response[] = "Error writing auth data\n";
                send(client_sock, response, strlen(response), 0);
                return;
            }
            
            char activity[100];
	        snprintf(activity, sizeof(activity), "edit nama %s menjadi %s", old_username, new_username);
            log_user_activity(channels[i].name, "root", activity);
        }
        
        char response[100];
        sprintf(response, "%s berhasil diubah menjadi %s\n", old_username, new_username);
        send(client_sock, response, strlen(response), 0);
        
    } else {
        char response[100];
        sprintf(response, "%s tidak ditemukan\n", old_username);
        send(client_sock, response, strlen(response), 0);
    }
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

void remove_user_from_channel_auth(const char *username) {
    DIR *dir = opendir("DiscorIT");
    
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char auth_file_path[BUFFER_SIZE];
            snprintf(auth_file_path, sizeof(auth_file_path), "%s/%s/admin/auth.csv", "DiscorIT", entry->d_name);

            FILE *auth_file = fopen(auth_file_path, "r");
            FILE *temp_file = fopen("temp_auth.csv", "w");

            if (!auth_file || !temp_file) {
                perror("Unable to open auth file or create temp file");
                if (auth_file) fclose(auth_file);
                if (temp_file) fclose(temp_file);
                continue;
            }

            char line[BUFFER_SIZE];
            while (fgets(line, sizeof(line), auth_file)) {
                int id;
                char stored_username[BUFFER_SIZE], role[BUFFER_SIZE];
                sscanf(line, "%d,%[^,],%s", &id, stored_username, role);
                if (strcmp(stored_username, username) != 0) {
                    fputs(line, temp_file);
                }
            }

            fclose(auth_file);
            fclose(temp_file);

            remove(auth_file_path);
            rename("temp_auth.csv", auth_file_path);

            char log_message[BUFFER_SIZE];
            snprintf(log_message, sizeof(log_message), "menghapus user %s dari channel", username);
            log_user_activity(entry->d_name,"root", log_message);
        }
    }
}

void remove_user(int client_sock, const char *username) {
    User users[100];
    int count = 0;

    if (!read_csv(users, &count)) {
        char response[] = "Error reading user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool user_found = false;
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].name, username) == 0) {
            for (int j = i; j < count - 1; j++) {
                users[j] = users[j + 1];
            }
            count--;
            user_found = true;
            break;
        }
    }

    if (user_found) {
        FILE *file = fopen(CSV_FILE, "r");
        FILE *temp = fopen("temp.csv", "w");
        if (!file || !temp) {
            perror("fopen");
            return;
        }

        char line[BUFFER_SIZE];
        int found = 0;
        while (fgets(line, sizeof(line), file)) {
            int id;
            char stored_username[BUFFER_SIZE], stored_password[BUFFER_SIZE], role[BUFFER_SIZE];
            sscanf(line, "%d,%[^,],%[^,],%s", &id, stored_username, stored_password, role);
            if (strcmp(stored_username, username) == 0) {
                found = 1;
            } else {
                fputs(line, temp);
            }
        }

        fclose(file);
        fclose(temp);

        if (found) {
            remove(CSV_FILE);
            rename("temp.csv", CSV_FILE);
            snprintf(line, sizeof(line), "User %s berhasil dihapus", username);
            remove_user_from_channel_auth(username);
        } else {
            remove("temp.csv");
            snprintf(line, sizeof(line), "User %s not found", username);
        }

        send(client_sock, line, strlen(line), 0);

    } else {
        char response[100];
        sprintf(response, "%s tidak ditemukan\n", username);
        send(client_sock, response, strlen(response), 0);
    }
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
    
    char activity[100];
    snprintf(activity, sizeof(activity), "berhasil mengubah %s menjadi %s", old_channel_name, new_channel_name);
    log_user_activity(new_channel_name, username, activity);
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
    int delete_result = remove_directory(channel_directory);
    if (delete_result == -1) {
        perror("remove_directory");
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

    // Update client state to reflect that the user is in a channel
    state->in_channel = true;
    strcpy(state->current_channel, channel_name);
    
        char auth_path[100];
    snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", channel_name);

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(channel_name, entries, &auth_count)) {
        //sprintf(response, "error reading auth data");
        
        return;
    }
    
     bool is_banned = false;
    for (int i = 0; i < auth_count; i++) {
        if (strcmp(entries[i].name, username) == 0 && strcmp(entries[i].role, "Banned") == 0) {
            is_banned = true;
            break;
        }
    }

    // If user is banned, send appropriate response
    if (is_banned) {
        char response[] = "Anda telah dibanned dari channel ini. Silakan hubungi admin untuk informasi lebih lanjut.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    } else {
    char response[100];
    sprintf(response, "%s bergabung ke channel %s\n", username, channel_name);
    send(client_sock, response, strlen(response), 0);
    }
    
    char activity[100];
    snprintf(activity, sizeof(activity), "masuk ke channel %s", channel_name);
    log_user_activity(channel_name, username, activity);

    // Check if user already exists in auth.csv
    for (int i = 0; i < auth_count; i++) {
        if (strcmp(entries[i].name, username) == 0) {
            //sprintf(response, "%s sudah terdaftar di dalam channel %s", username, channel_name);
            
            return;
        }
    }

    // Add new entry for the user
    AuthEntry new_entry;
    new_entry.id_user = auth_count + 1; // Assuming id_user is sequentially assigned
    strcpy(new_entry.name, username);
    if (is_root_user(username)) {
        strcpy(new_entry.role, "ROOT");
    } else {
        strcpy(new_entry.role, "USER");
    }

    entries[auth_count++] = new_entry;

    // Write back to auth.csv
    if (!write_auth(channel_name, entries, &auth_count)) {
        char response[] = "Error writing auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }
}



void create_room(int client_sock, ClientState *state, const char *username, const char *room_name) {
  // Check if the user is an admin in the specified channel
    char auth_path[100];
    snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", state->current_channel);

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(state->current_channel, entries, &auth_count)) {
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
        char response[] = "Anda tidak memiliki izin untuk membuat room\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }
    
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
        sprintf(response, "Room %s dibuat\n", room_name);
        send(client_sock, response, strlen(response), 0);
    }
    
    char activity[100];
    snprintf(activity, sizeof(activity), "buat room %s", room_name);
    log_user_activity(state->current_channel, username, activity);
}


void rename_room(int client_sock, ClientState *state, const char *username, const char *old_room_name, const char *new_room_name) {
      // Check if the user is an admin in the specified channel
    char auth_path[100];
    snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", state->current_channel);

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(state->current_channel, entries, &auth_count)) {
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
        char response[] = "Anda tidak memiliki izin untuk mengedit room\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }
    
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
    snprintf(response, sizeof(response), "Room %s berhasil diubah menjadi %s\n", old_room_name, new_room_name);
    send(client_sock, response, strlen(response), 0);
    
    char activity[100];
    snprintf(activity, sizeof(activity), "edit room %s menjadi %s", old_room_name, new_room_name);
    log_user_activity(state->current_channel, username, activity);
}

void delete_room(int client_sock, ClientState *state, const char *username, const char *room_name) {
    char auth_path[100];
    snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", state->current_channel);

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(state->current_channel, entries, &auth_count)) {
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
        char response[] = "Anda tidak memiliki izin untuk menghapus room\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }
    
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
    snprintf(response, sizeof(response), "%s berhasil dihapus\n", room_name);
    send(client_sock, response, strlen(response), 0);
    
    char activity[100];
    snprintf(activity, sizeof(activity), "delete room %s", room_name);
    log_user_activity(state->current_channel, username, activity);
}

void delete_all_rooms(int client_sock, ClientState *state, const char *username, const char *channel_name) {
    char auth_path[100];
    snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", state->current_channel);

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(state->current_channel, entries, &auth_count)) {
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
        char response[] = "Anda tidak memiliki izin untuk menghapus room\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    if (!state->in_channel) {
        char response[] = "Anda harus bergabung dengan sebuah channel sebelum menghapus room.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    DIR *dir;
    struct dirent *entry;

    // Construct path to channel's rooms directory
    char channel_path[256];
    snprintf(channel_path, sizeof(channel_path), "DiscorIT/%s", state->current_channel);

    // Open the directory
    if ((dir = opendir(channel_path)) == NULL) {
        char response[100];
        sprintf(response, "Gagal membuka direktori channel '%s'\n", state->current_channel);
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Iterate over entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if(strcmp(entry->d_name, "admin") == 0) {
                continue;
            }
            char room_dir_path[BUFFER_SIZE];
            snprintf(room_dir_path, sizeof(room_dir_path), "%s/%s", channel_path, entry->d_name);

            int delete_result = remove_directory(room_dir_path);
            if (delete_result == -1) {
                perror("remove_directory");
                write(socket, "Failed to delete room directory", strlen("Failed to delete room directory"));
                return;
            }
        }
    }

    char response[100];
    snprintf(response, sizeof(response), "Semua room berhasil dihapus\n");
    send(client_sock, response, strlen(response), 0);

    closedir(dir);

    char activity[100];
    snprintf(activity, sizeof(activity), "delete all room");
    log_user_activity(state->current_channel, username, activity);
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
    }
    
    char response[100];
    sprintf(response, "Bergabung ke room %s\n", room_name);
    send(client_sock, response, strlen(response), 0);
    
    char activity[100];
    snprintf(activity, sizeof(activity), "masuk ke room %s", room_name);
    log_user_activity(state->current_channel, state->username, activity);

}


void list_rooms(int client_sock, ClientState *state) {
    char channel_path[256];
    snprintf(channel_path, sizeof(channel_path), "DiscorIT/%s", state->current_channel);

    DIR *dir;
    struct dirent *entry;

    // Open the directory
    if ((dir = opendir(channel_path)) == NULL) {
        char response[100];
        sprintf(response, "Gagal membuka direktori channel '%s'\n", state->current_channel);
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[1024] = "";

    // Iterate over entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue; // Skip current and parent directory entries
            }

            // Exclude room with name "admin"
            if (strcmp(entry->d_name, "admin") == 0) {
                continue;
            }

            strcat(response, entry->d_name);
            strcat(response, " ");
        }
    }
    strcat(response, "\n");

    closedir(dir);

    // Send the response to client
    send(client_sock, response, strlen(response), 0);
}

bool is_admin_or_root(const char *username, const char *channel_name) {
    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(channel_name, entries, &auth_count)) {
        return false;
    }

    for (int i = 0; i < auth_count; i++) {
        if (strcmp(entries[i].name, username) == 0 && 
            (strcmp(entries[i].role, "ADMIN") == 0 || strcmp(entries[i].role, "ROOT") == 0)) {
            return true;
        }
    }
    return false;
}

void ban_user(int client_sock, ClientState *state, const char *username, const char *channel_name) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before joining a room.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    if (!is_admin_or_root(state->username, state->current_channel)) {
        char response[] = "You do not have permission to ban users.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(state->current_channel, entries, &auth_count)) {
        char response[] = "Error reading auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool user_found = false;
    for (int i = 0; i < auth_count; i++) {
        if (strcmp(entries[i].name, username) == 0) {
            strcpy(entries[i].role, "Banned");
            user_found = true;
            break;
        }
    }

    if (!user_found) {
        char response[] = "User not found in channel\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    if (!write_auth(channel_name, entries, &auth_count)) {
        char response[] = "Error writing auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[100];
    sprintf(response, "%s diban\n", username);
    send(client_sock, response, strlen(response), 0);
    
    char activity[100];
    snprintf(activity, sizeof(activity), "ban %s", username);
    log_user_activity(channel_name, state->username, activity);
}


void unban_user(int client_sock, ClientState *state, const char *username, const char *channel_name) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before joining a room.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }


    if (!is_admin_or_root(state->username, state->current_channel)) {
        char response[] = "You do not have permission to unban users.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(state->current_channel, entries, &auth_count)) {
        char response[] = "Error reading auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool user_found = false;
    for (int i = 0; i < auth_count; i++) {
        if (strcmp(entries[i].name, username) == 0 && strcmp(entries[i].role, "Banned") == 0) {
            strcpy(entries[i].role, "USER");
            user_found = true;
            break;
        }
    }

    if (!user_found) {
        char response[] = "User not found or not banned in channel\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    if (!write_auth(state->current_channel, entries, &auth_count)) {
        char response[] = "Error writing auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[100];
    sprintf(response, "%s telah diunban\n", username);
    send(client_sock, response, strlen(response), 0);
    
    char activity[100];
    snprintf(activity, sizeof(activity), "unban %s", username);
    log_user_activity(channel_name, state->username, activity);
}

void remove_user_from_channel(int client_sock, ClientState *state, const char *username, const char *channel_name) {
     if (!state->in_channel) {
        char response[] = "You must join a channel before joining a room.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }


    if (!is_admin_or_root(state->username, state->current_channel)) {
        char response[] = "You do not have permission to remove users.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    AuthEntry entries[10];
    int auth_count = 0;

    if (!read_auth_confirm(state->current_channel, entries, &auth_count)) {
        char response[] = "Error reading auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool user_found = false;
    for (int i = 0; i < auth_count; i++) {
        if (strcmp(entries[i].name, username) == 0) {
            for (int j = i; j < auth_count - 1; j++) {
                entries[j] = entries[j + 1];
            }
            auth_count--;
            user_found = true;
            break;
        }
    }

    if (!user_found) {
        char response[] = "User not found in channel\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    if (!write_auth(state->current_channel, entries, &auth_count)) {
        char response[] = "Error writing auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[100];
    sprintf(response, "%s telah dihapus dari channel\n", username);
    send(client_sock, response, strlen(response), 0);
    
    char activity[100];
    snprintf(activity, sizeof(activity), "remove user %s", username);
    log_user_activity(state->current_channel, state->username, activity);
}

void edit_profile_self(int client_sock, ClientState *state, const char *username, const char *new_username) {
    // Update username in users.csv
    User users[100];
    int user_count = 0;

    if (!read_csv(users, &user_count)) {
        char response[] = "Error reading user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool user_found = false;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, username) == 0) {
            strcpy(users[i].name, new_username);
            user_found = true;
            break;
        }
    }

    if (!user_found) {
        char response[] = "User not found\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Write updated users.csv
    if (!write_csv(users, user_count)) {
        char response[] = "Error writing user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Update username in auth.csv for all channels
    Channel channels[100];
    int channel_count = 0;

    if (!read_channels(channels, &channel_count)) {
        char response[] = "Error reading channel data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    for (int i = 0; i < channel_count; i++) {
        char auth_path[100];
        snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", channels[i].name);

        AuthEntry entries[10];
        int auth_count = 0;

        if (!read_auth_confirm(channels[i].name, entries, &auth_count)) {
            continue; // Skip to next channel if auth.csv reading fails
        }

        // Update username in auth.csv entries
        for (int j = 0; j < auth_count; j++) {
            if (strcmp(entries[j].name, username) == 0) {
                strcpy(entries[j].name, new_username);
            }
        }

        // Write back updated auth.csv
        if (!write_auth(channels[i].name, entries, &auth_count)) {
            char response[] = "Error writing auth data\n";
            send(client_sock, response, strlen(response), 0);
            return;
        }
        
        char activity[100];
	snprintf(activity, sizeof(activity), "edit nama %s menjadi %s", username, new_username);
        log_user_activity(channels[i].name, username, activity);
        
    }

    char response[] = "Profil diupdate\n";
    send(client_sock, response, strlen(response), 0);
    
 
}


void edit_password_self(int client_sock, ClientState *state, const char *username, const char *new_password) {
    User users[100];
    int count = 0;

    if (!read_csv(users, &count)) {
        char response[] = "Error reading user data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].name, username) == 0) {
            // Update password using bcrypt
            if (bcrypt_hash(new_password, users[i].password)) {
                if (write_csv(users, count)) {
                    char response[] = "Password diupdate\n";
                    send(client_sock, response, strlen(response), 0);
                    return;
                } else {
                    char response[] = "Error updating password\n";
                    send(client_sock, response, strlen(response), 0);
                    return;
                }
            } else {
                char response[] = "Error hashing password\n";
                send(client_sock, response, strlen(response), 0);
                return;
            }
        }
    }

    char response[] = "User not found\n";
    send(client_sock, response, strlen(response), 0);
}

void list_users_in_channel(int client_sock, const char *channel_name) {
    AuthEntry entries[10];
    int auth_count = 0;

    char auth_path[100];
    snprintf(auth_path, sizeof(auth_path), "DiscorIT/%s/admin/auth.csv", channel_name);

    if (!read_auth_confirm(channel_name, entries, &auth_count)) {
        char response[] = "Error reading auth data\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[BUFFER_SIZE];
    response[0] = '\0';  // Ensure response starts empty
    for (int i = 0; i < auth_count; ++i) {
        strcat(response, entries[i].name);
        strcat(response, " ");
    }
    strcat(response, "\n");
    send(client_sock, response, strlen(response), 0);
}

int get_next_chat_id(const char *chat_path) {
    FILE *chat_file = fopen(chat_path, "r");
    if (chat_file == NULL) {
        // Handle error opening file
        fprintf(stderr, "Error opening chat file %s\n", chat_path);
        return -1; // Return error code or handle appropriately
    }

    int max_id = 0;
    char line[100]; // Adjust buffer size as necessary

    while (fgets(line, sizeof(line), chat_file) != NULL) {
        int chat_id;
        if (sscanf(line, "[%*[^]]][%d]", &chat_id) == 1) {
            if (chat_id > max_id) {
                max_id = chat_id;
            }
        }
    }

    fclose(chat_file);

    // Return next available ID
    return max_id + 1;
}

void send_chat_message(int client_sock, ClientState *state, const char *message_text) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before sending messages.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char chat_path[256];
    snprintf(chat_path, sizeof(chat_path), "DiscorIT/%s/%s/chat.csv", state->current_channel, state->current_room);

    FILE *fp = fopen(chat_path, "a+");
    if (fp == NULL) {
        char response[] = "Error opening chat file\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    int next_id = get_next_chat_id(chat_path);
    if (next_id == -1) {
        char response[] = "Error getting next chat ID\n";
        send(client_sock, response, strlen(response), 0);
        fclose(fp);
        return;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", tm_info);

    fprintf(fp, "[%s][%d][%s] \"%s\"\n", timestamp, next_id, state->username, message_text);

    fclose(fp);

    char response[] = "Pesan terkirim\n";
    send(client_sock, response, strlen(response), 0);
}


void see_chat_messages(int client_sock, ClientState *state) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before viewing messages.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char chat_path[256];
    snprintf(chat_path, sizeof(chat_path), "DiscorIT/%s/%s/chat.csv", state->current_channel, state->current_room);

    FILE *fp = fopen(chat_path, "r");
    if (fp == NULL) {
        char response[] = "Error opening chat file\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char buffer[1000]; // adjust size as needed
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        send(client_sock, buffer, bytes_read, 0);
    }

    fclose(fp);
}

void edit_chat_message(int client_sock, ClientState *state, int message_id, const char *new_message) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before editing messages.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char chat_path[256];
    snprintf(chat_path, sizeof(chat_path), "DiscorIT/%s/%s/chat.csv", state->current_channel, state->current_room);

    FILE *fp_read = fopen(chat_path, "r");
    if (fp_read == NULL) {
        char response[] = "Error opening chat file\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    FILE *fp_write = fopen("temp_chat.csv", "w");
    if (fp_write == NULL) {
        char response[] = "Error creating temporary file\n";
        fclose(fp_read);
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool message_found = false;
    char line[256];

    while (fgets(line, sizeof(line), fp_read) != NULL) {
        int current_id;
        char username[50], message_text[200], timestamp[20];

        if (sscanf(line, "[%19[^]]][%d][%49[^]]] \"%199[^\"]\"", timestamp, &current_id, username, message_text) == 4) {
            if (current_id == message_id && strcmp(username, state->username) == 0) {
                fprintf(fp_write, "[%s][%d][%s] \"%s\"\n", timestamp, current_id, username, new_message);
                message_found = true;
            } else {
                fprintf(fp_write, "[%s][%d][%s] \"%s\"\n", timestamp, current_id, username, message_text);
            }
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    if (!message_found) {
        remove("temp_chat.csv");
        char response[] = "Message ID not found or you are not the author of the message\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Rename temp file to original file
    if (rename("temp_chat.csv", chat_path) != 0) {
        char response[] = "Error updating chat file\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[] = "Pesan berhasil diedit\n";
    send(client_sock, response, strlen(response), 0);
}

void delete_chat_message(int client_sock, ClientState *state, int message_id) {
    if (!state->in_channel) {
        char response[] = "You must join a channel before deleting messages.\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char chat_path[256];
    snprintf(chat_path, sizeof(chat_path), "DiscorIT/%s/%s/chat.csv", state->current_channel, state->current_room);

    FILE *fp_read = fopen(chat_path, "r");
    if (fp_read == NULL) {
        char response[] = "Error opening chat file\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    FILE *fp_write = fopen("temp_chat.csv", "w");
    if (fp_write == NULL) {
        char response[] = "Error creating temporary file\n";
        fclose(fp_read);
        send(client_sock, response, strlen(response), 0);
        return;
    }

    bool message_found = false;
    char line[256];

    while (fgets(line, sizeof(line), fp_read) != NULL) {
        int current_id;
        char username[50], message_text[200], timestamp[20];

        if (sscanf(line, "[%19[^]]][%d][%49[^]]] \"%199[^\"]\"", timestamp, &current_id, username, message_text) == 4) {
            if (current_id == message_id && strcmp(username, state->username) == 0) {
                message_found = true;
            } else {
                fprintf(fp_write, "[%s][%d][%s] \"%s\"\n", timestamp, current_id, username, message_text);
            }
        }
    }

    fclose(fp_read);
    fclose(fp_write);

    if (!message_found) {
        remove("temp_chat.csv");
        char response[] = "Message ID not found or you are not the author of the message\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    // Rename temp file to original file
    if (rename("temp_chat.csv", chat_path) != 0) {
        char response[] = "Error updating chat file\n";
        send(client_sock, response, strlen(response), 0);
        return;
    }

    char response[] = "Pesan berhasil dihapus.\n";
    send(client_sock, response, strlen(response), 0);
}



void handle_client(int client_sock) 
{
    char buffer[BUFFER_SIZE];
    int bytes_received;
    ClientState state = {0};

    while ((bytes_received = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) 
    {
        buffer[bytes_received] = '\0';

        char command[BUFFER_SIZE], username[50], password[50];
        sscanf(buffer, "%s %s -p %s", command, username, password);

        if (strcmp(command, "REGISTER") == 0) 
        {
            register_user(client_sock, username, password);
        } 
        
        else if (strcmp(command, "LOGIN") == 0) 
        {
            User users[100];
            int count = 0;

            if (!read_csv(users, &count)) 
            {
                char response[] = "Error reading user data\n";
                send(client_sock, response, strlen(response), 0);
                continue;
            }

            bool user_found = false;
            bool is_root = false;
            for (int i = 0; i < count; i++) 
            {
                if (strcmp(users[i].name, username) == 0) 
                {
                    user_found = true;
                    is_root = strcmp(users[i].role, "ROOT") == 0;
                 
                    if (bcrypt_check(password, users[i].password)) 
                    {
                        state.is_authenticated = true;
                        strcpy(state.username, username);
                    
                        char response[100];
                        sprintf(response, "%s berhasil login\n", username);
                        send(client_sock, response, strlen(response), 0);

                        while (1) 
                        {
                            bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
                        
                            if (bytes_received <= 0) 
                            {
                                break;
                            }
                            buffer[bytes_received] = '\0';

                            if (strcmp(buffer, "LIST USER") == 0) 
                            {
                            	if (state.in_channel) 
                                {
                                    list_users_in_channel(client_sock, state.current_channel);
                            	} 
                                
                                else 
                                {
                                    if (is_root) {
                                        list_users(client_sock);
                                    } 
                                    else 
                                    {
                                        char response[] = "Anda tidak memiliki izin untuk melihat daftar pengguna\n";
                                        send(client_sock, response, strlen(response), 0);
                                    }
                                }
                            } 
                            
                            else if (strncmp(buffer, "EDIT WHERE ", 11) == 0) 
                            {
                                if (is_root) 
                                {
                                    char option[10], old_username[50], new_username[50], new_value[50];
                                    sscanf(buffer, "EDIT WHERE %s -%s %s", old_username, option, new_value);
                                
                                    if (strcmp(option, "u") == 0) 
                                    {
                                        edit_user(client_sock, old_username, new_value);
                                        char activity[100];
                                        
                                        snprintf(activity, sizeof(activity), "berhasil mengubah %s menjadi %s", old_username, new_username);
                                        log_user_activity(state.current_channel, "root", activity);
                                    } 
                                    
                                    else if (strcmp(option, "p") == 0) 
                                    {
                                        edit_password(client_sock, old_username, new_value);
                                    } 
                                    
                                    else 
                                    {
                                        char response[] = "Invalid EDIT command\n";
                                        send(client_sock, response, strlen(response), 0);
                                    }
                                } 
                                
                                else 
                                {
                                    char response[] = "Anda tidak memiliki izin untuk mengedit pengguna\n";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } 
                            
                            else if (strncmp(buffer, "REMOVE USER ", 12) == 0) 
                            {
                         	    if (state.in_channel) 
                                {
                                    char channel_name[50], username_to_remove[50];
                                    sscanf(buffer, "REMOVE USER %s FROM %s", username_to_remove, channel_name);
                                    remove_user_from_channel(client_sock, &state, username_to_remove, channel_name);
                                } 
                                
                                else 
                                {
                                    char response[]="Anda harus bergabung ke dalam channel";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } 
                            
                            else if (strncmp(buffer, "REMOVE ", 7) == 0) 
                            {
                                if (is_root) 
                                {
                                    char username_to_remove[50];
                                    sscanf(buffer, "REMOVE %s", username_to_remove);
                                    remove_user(client_sock, username_to_remove);
                                }
                                
                                else 
                                {
                                    char response[] = "Anda tidak memiliki izin untuk remove pengguna\n";
                                
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } 
                            
                            else if (strncmp(buffer, "CREATE CHANNEL ", 15) == 0) 
                            {
                                char option[10], channel_name[50], key[50];
                                sscanf(buffer, "CREATE CHANNEL %s -%s %s", channel_name, option, key);
                            
                                if (strcmp(option, "k") == 0) 
                                {
                                    create_channel(client_sock, username, channel_name, key);
                                } 
                                
                                else 
                                {
                                    char response[] = "Invalid CREATE command\n";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } 
                            
                            else if (strncmp(buffer, "EDIT CHANNEL ", 13) == 0) 
                            {
                                char old_channel_name[50], new_channel_name[50];
                                int scanned = sscanf(buffer, "EDIT CHANNEL %s TO %s", 
                                old_channel_name, new_channel_name);
                                
                                if (scanned == 2) 
                                {
                                    edit_channel_name(client_sock, old_channel_name, new_channel_name, username);
                                } 
                                
                                else 
                                {
                                    char response[] = "Invalid EDIT CHANNEL command\n";
                                
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } 
                            
                            else if (strncmp(buffer, "DEL CHANNEL ", 11) == 0) 
                            {
                                char channel_name[50];
                                sscanf(buffer, "DEL CHANNEL %s", channel_name);
                                delete_channel(client_sock, channel_name, username);
                            } 
                            
                            else if (strcmp(buffer, "LIST CHANNEL") == 0) 
                            {
                                list_channels(client_sock);
                            } 
                            
                            else if (strncmp(buffer, "JOIN ", 5) == 0) 
                            {
                            	if (state.in_channel) 
                                {
                                    char room_name[50];
                                    sscanf(buffer, "JOIN %s", room_name);
                                    join_room(client_sock, &state, room_name);
                                    
                                    state.in_room = 1;
                            	}
                            	
                                else 
                                {
                                    char channel_name[50];
                                    sscanf(buffer, "JOIN %s", channel_name);
                                    join_channel(client_sock, &state, username, channel_name);
                                
                                    state.in_channel = 1;
                                }
                            } 
                            
                            else if (strncmp(buffer, "CREATE ROOM ", 12) == 0) 
                            {
                                char room_name[50];
                                sscanf(buffer, "CREATE ROOM %s", room_name);
                                create_room(client_sock, &state, username, room_name);
                            } 
                            
                            else if (strncmp(buffer, "EDIT ROOM ", 10) == 0) 
                            {
                                char old_room_name[50], new_room_name[50];
                                int scanned = sscanf(buffer, "EDIT ROOM %s TO %s", old_room_name, new_room_name);
                                if (scanned == 2) 
                                {
                                    rename_room(client_sock, &state, state.username, old_room_name, new_room_name);
                                } 
                                
                                else 
                                {
                                    char response[] = "Invalid EDIT ROOM command format\n";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            } 
                            
                            else if (strncmp(buffer, "DEL ROOM ", 9) == 0 || strcmp(buffer, "DEL ROOM ALL") == 0) 
                            {
                                char room_name[50], channel_name[50];
                                if (strcmp(buffer, "DEL ROOM ALL") == 0) 
                                {
                                    delete_all_rooms(client_sock, &state, username, channel_name);
                                } 
                                
                                else 
                                {
                                    sscanf(buffer, "DEL ROOM %s", room_name);
                                    delete_room(client_sock, &state, username, room_name);
                                }
                            } 
                            
                            else if (strncmp(buffer, "BAN ", 4) == 0) 
                            {
                                char username_to_ban[50], channel_name[50];
                                sscanf(buffer, "BAN %s", username_to_ban);
                                ban_user(client_sock, &state, username_to_ban, channel_name);
                            } 
                            
                            else if (strncmp(buffer, "UNBAN ", 4) == 0) 
                            {
                                char username_to_unban[50], channel_name[50];
                                sscanf(buffer, "UNBAN %s", username_to_unban);
                                unban_user(client_sock, &state, username_to_unban, channel_name);             
                            } 
                            
                            else if (strcmp(buffer, "LIST ROOM") == 0) 
                            {
                                list_rooms(client_sock, &state);
			                } 
                            
                            else if (strncmp(buffer, "EDIT PROFILE SELF -u ", 21) == 0) 
                            {
                            char new_username[50];
                                sscanf(buffer, "EDIT PROFILE SELF -u %s", new_username);
                                edit_profile_self(client_sock, &state, state.username, new_username);
                            } 
                            
                            else if (strncmp(buffer, "EDIT PROFILE SELF -p ", 21) == 0) 
                            {
                                char new_value[50];
                                sscanf(buffer, "EDIT PROFILE SELF -p %s", new_value);
                                edit_password_self(client_sock, &state, username, new_value);
                            } 
                            
                            else if (strncmp(buffer, "CHAT ", 5) == 0) 
                            {
                                char text[BUFFER_SIZE], channel_name[50], room_name[50], username[50];
                                sscanf(buffer, "CHAT \"%[^\"]\"", text);
                                send_chat_message(client_sock, &state, text);
                            } 
                            
                            else if (strncmp(buffer, "SEE CHAT", 8) == 0) 
                            {
                                char channel_name[50], room_name[50];
                                see_chat_messages(client_sock, &state);
                            } 
                            
                            else if (strncmp(buffer, "EDIT CHAT ", 10) == 0) 
                            {
                                int chat_id;
                                char new_text[BUFFER_SIZE], channel_name[50], room_name[50];
                                if (sscanf(buffer, "EDIT CHAT %d \"%[^\"]\"", &chat_id, new_text) == 2) 
                                {
                                edit_chat_message(client_sock, &state, chat_id, new_text);
                                } 
                                
                                else 
                                {
                                char response[] = "Invalid EDIT CHAT command format\n";
                                send(client_sock, response, strlen(response), 0);
                                }
                            } 
                            
                            else if (strncmp(buffer, "DEL CHAT ", 9) == 0) 
                            {
                                int chat_id;
                                if (sscanf(buffer, "DEL CHAT %d", &chat_id) == 1) 
                                {
                                    delete_chat_message(client_sock, &state, chat_id);
                                } 
                                
                                else 
                                {
                                    char response[] = "Invalid DEL CHAT command format\n";
                                    send(client_sock, response, strlen(response), 0);
                                }
                            
                            } 
                            
                            else if (strcmp(buffer, "EXIT") == 0) 
                            {
                            	if (state.in_room) 
                                {
                                    state.in_room = false;
                                    char response[100];
                                 
                                    sprintf(response, "%s berhasil keluar dari room %s\n", username, state.current_room);
                                    send(client_sock, response, strlen(response), 0);
                            	}
                            	
                                else if (state.in_channel) 
                                {
                                    state.in_channel = false;
                                    
                                    char response[100];
                                    sprintf(response, "%s berhasil keluar dari channel %s\n", username, state.current_channel);
                                    send(client_sock, response, strlen(response), 0);
                            	}
                            	
                                else 
                                {
                                    state.is_authenticated = false;
                                    state.in_channel = false;
                                
                                    char response[100];
                                    sprintf(response, "%s berhasil logout\n", username);
                                    send(client_sock, response, strlen(response), 0);
                                
                                    break;
                                }
                            } 
                            
                            else 
                            {
                                char response[] = "Command not recognized\n";
                                send(client_sock, response, strlen(response), 0);
                            }
                        }
                    } 
                    
                    else 
                    {
                        char response[] = "Password salah\n";
                        send(client_sock, response, strlen(response), 0);
                    
                        break;
                    }
                    
                    break;
                }
            }

            if (!user_found) 
            {
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
    
     if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEPORT) failed");
        close(server_sock);
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
```

## monitor.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define CHAT_FILE_PREFIX "DiscorIT/"
#define CHAT_FILE_SUFFIX "/chat.csv"

pthread_t monitor_thread;
int monitor_active = 0;
int sock;
char username[BUFFER_SIZE];
char currentChannel[BUFFER_SIZE] = "";
char currentRoom[BUFFER_SIZE] = "";

void send_command(int sock, const char *command) {
    char buffer[BUFFER_SIZE];

    send(sock, command, strlen(command), 0);
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("%s\n", buffer);
    }
}

// Di dalam fungsi monitor_chat
void *monitor_chat(void *arg) {
    char *channel_name = ((char **)arg)[0];
    char *room_name = ((char **)arg)[1];

    char chat_path[BUFFER_SIZE];
    snprintf(chat_path, sizeof(chat_path), "%s%s/%s%s", CHAT_FILE_PREFIX, channel_name, room_name, CHAT_FILE_SUFFIX);
    
    printf("Monitoring file: %s\n", chat_path);  // Debug message

    struct stat st;
    time_t last_modified_time = 0;

    while (monitor_active) {
        if (stat(chat_path, &st) == -1) {
            perror("Error stating file");
            sleep(1);
            continue;
        }

        if (st.st_mtime > last_modified_time) {
            printf("File modified, reading contents...\n");  // Debug message
            
            FILE *chat_file = fopen(chat_path, "r");
            if (chat_file == NULL) {
                perror("Failed to open chat file");
                sleep(1);
                continue;
            }

            char line[BUFFER_SIZE];
            while (fgets(line, sizeof(line), chat_file)) {
                line[strcspn(line, "\n")] = 0;
                printf("%s\n", line);
            }
            
            fclose(chat_file);
            
            last_modified_time = st.st_mtime;
            
            printf("[%s/%s/%s] ", username, currentChannel, currentRoom);
            fflush(stdout);
        }

        sleep(1);
    }

    printf("Stopped monitoring %s\n", chat_path);  // Debug message
    return NULL;
}

void handle_commands(int sock, char *username) {
    char command[BUFFER_SIZE];
    int loggedIn = 1;

    while (loggedIn) {
        printf("[%s] ", username);

        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        send(sock, command, strlen(command), 0);

        if (strstr(command, "-channel")) {
            sscanf(command, " -channel %s -room %s", currentChannel, currentRoom);

            // Hentikan thread monitor yang sedang berjalan jika ada
            if (monitor_active) {
                monitor_active = 0;
                pthread_join(monitor_thread, NULL);
            }

            // Mulai thread monitor baru
            char *monitor_args[] = { currentChannel, currentRoom };
            monitor_active = 1;
            if (pthread_create(&monitor_thread, NULL, monitor_chat, (void*)monitor_args) != 0) {
                perror("Failed to create monitor thread");
                close(sock);
                exit(EXIT_FAILURE);
            }
            printf("Started monitoring channel %s, room %s\n", currentChannel, currentRoom);
        }

        if (strstr(command, "EXIT")) {
            monitor_active = 0;
            pthread_join(monitor_thread, NULL);
            loggedIn = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5 || (strcmp(argv[1], "REGISTER") != 0 && strcmp(argv[1], "LOGIN") != 0)) {
        printf("Usage:\n");
        printf("./monitor REGISTER username -p password\n");
        printf("./monitor LOGIN username -p password\n");
        return 1;
    }

    strcpy(username, argv[2]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
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
```

## Penjelasan Kode

### discorit.c

Kode `discorit.c` adalah program client untuk berinteraksi dengan server menggunakan socket TCP. Program ini memungkinkan pengguna untuk mendaftar atau masuk ke sistem, dan kemudian mengirim dan menerima perintah dari server.

Ada 3 fungsi di dalam kode `discorit.c`:

#### Fungsi `send_command`

```c
void send_command(int sock, const char *command) {
    char buffer[BUFFER_SIZE];

    send(sock, command, strlen(command), 0);
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("%s\n", buffer);
    }
}
```

- `send_command` mengirimkan perintah `command` ke server menggunakan socket `sock`.
- Setelah mengirimkan perintah, fungsi menerima respons dari server dan mencetaknya ke layar.

#### Fungsi `handle_commands`

```c
void handle_commands(int sock, char *username) {
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
                    room_name += strlen("bergabung ke room ");
                    sscanf(room_name, "%s", currentRoom);
                    printf("[%s/%s/%s] %s\n", username, currentChannel, currentRoom, buffer);
                } else {
                    printf("%s\n", buffer); // Default print if room name not found
                }
            } else if (strstr(buffer, "Profil diupdate")) {
                // Extract new_username from the command entered
                char new_username[50];
                sscanf(command, "EDIT PROFILE SELF -u %s", new_username);
                strncpy(username, new_username, BUFFER_SIZE - 1); // Update the username
                username[BUFFER_SIZE - 1] = '\0'; // Ensure null-terminated
                printf("Profil diupdate\n");
            } else if (strstr(buffer, "Bergabung ke room ")) {
                char *room_name = strstr(buffer, "Bergabung ke room ");
                if (room_name != NULL) {
                    room_name += strlen("Bergabung ke room ");
                    sscanf(room_name, "%s", currentRoom);
                    printf("[%s/%s/%s] %s\n", username, currentChannel, currentRoom, buffer);
                } else {
                    printf("%s\n", buffer); // Default print if room name not found
                }
            } else {
                printf("%s\n", buffer);
            }
        }

        if (strstr(buffer, "berhasil logout")) {
            loggedIn = 0;
        } else if (strstr(buffer, "berhasil keluar dari channel")) {
            currentChannel[0] = '\0';
        } else if (strstr(buffer, "berhasil keluar dari room")) {
            currentRoom[0] = '\0';
        }
    }
}
```

- `handle_commands` mengelola sesi setelah pengguna berhasil masuk.
- Menampilkan prompt sesuai dengan status pengguna (username, channel, dan room).
- Mengirim perintah pengguna ke server dan menerima respons.
- Mengelola respons dari server, seperti pembaruan profil, bergabung ke channel, atau room, serta keluar dari sistem.

#### Fungsi `main`

```c
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
```

- Mengecek argumen yang diberikan harus berupa `REGISTER` atau `LOGIN` dengan username dan password.
- Membuat socket TCP.
- Mengatur alamat server dan port.
- Menghubungkan ke server.
- Mengirim perintah register/login ke server.
- Jika login berhasil, memanggil `handle_commands` untuk mengelola perintah selanjutnya.
- Menutup socket setelah selesai.

#### Penanganan Kesalahan

- Kesalahan Argumen: Memeriksa jumlah dan tipe argumen yang diberikan. Jika tidak sesuai, memberikan pesan penggunaan yang benar dan keluar.
- Kesalahan Pembuatan Socket: Jika pembuatan socket gagal, mencetak pesan kesalahan dan keluar.
- Kesalahan Alamat IP: Jika alamat IP tidak valid, mencetak pesan kesalahan dan menutup socket.
- Kesalahan Koneksi: Jika koneksi ke server gagal, mencetak pesan kesalahan dan menutup socket.

### server.c

1. **Autentikasi Pengguna**

- Fitur: Pengguna dapat melakukan login menggunakan username dan password mereka.
- Implementasi:
    * Server memverifikasi password menggunakan fungsi hash bcrypt.
    * Pengguna juga dapat mendaftar (register) untuk membuat akun baru.

2. **Manajemen Pengguna**

- Fitur: Administrasi pengguna seperti edit username, edit password, dan penghapusan pengguna.
- Implementasi:
    - Fungsi `edit_user()` dan `edit_password()` digunakan untuk mengubah data pengguna dalam file CSV.
    - Fungsi `remove_user()` digunakan untuk menghapus pengguna dari sistem.

3. **Manajemen Channel dan Ruangan**

- Fitur: Pembuatan, pengeditan nama, dan penghapusan channel. Pembuatan, pengeditan nama, dan penghapusan ruangan dalam channel.
- Implementasi:
    - Fungsi `create_channel()`, `edit_channel_name()`, dan `delete_channel()` untuk manajemen channel.
    - Fungsi `create_room()`, `rename_room()`, `delete_room()`, dan `delete_all_rooms()` untuk manajemen ruangan dalam channel.

4. **Pertukaran Pesan**

- Fitur: Mengirim, melihat, mengedit, dan menghapus pesan dalam channel dan ruangan.
- Implementasi:
    - Fungsi `send_chat_message()` untuk mengirim pesan baru.
    - Fungsi `see_chat_messages()` untuk melihat pesan yang ada.
    - Fungsi `edit_chat_message()` dan `delete_chat_message()` untuk mengedit dan menghapus pesan yang sudah ada.

5. **Manajemen Keseluruhan**

- Fitur: Pemrosesan perintah, logging aktivitas pengguna, penanganan peran pengguna.
- Implementasi:
    - Server memproses berbagai perintah dari klien seperti `LIST USER`, `LIST CHANNEL`, `JOIN`, `EXIT`, dan lain-lain.
    - Logging aktivitas pengguna dalam file atau database untuk audit dan pemantauan.
    - Validasi peran pengguna (seperti peran "ROOT") untuk membatasi akses ke beberapa operasi tertentu.

6. **Penanganan Kesalahan**

- Fitur: Memberikan pesan kesalahan yang jelas dan menangani kondisi tak terduga dengan benar.
- Implementasi:
    - Menggunakan perror() untuk menampilkan pesan kesalahan sistem yang terkait dengan operasi soket dan file.
    - Mengirim pesan balasan ke klien yang menjelaskan alasan kegagalan operasi tertentu, misalnya, gagal login karena password salah atau gagal membaca data pengguna.

#### Struktur File

1. **server.c**

- **Deskripsi**: File sumber utama aplikasi server, mungkin berisi fungsi `main()` dan fungsi-fungsi utama untuk inisialisasi server, penanganan koneksi, dan operasi-operasi utama seperti autentikasi pengguna, manajemen channel, dan pengiriman pesan.
- **Fungsi Utama**: Membuat soket server, mengatur koneksi, menerima dan menangani koneksi klien, serta menanggapi perintah-perintah dari klien seperti login, pembuatan channel, pengiriman pesan, dll.

2. **users.csv**

- **Deskripsi**: File CSV yang berisi data pengguna yang terdaftar dalam aplikasi. Setiap baris mungkin berisi informasi seperti username, password terenkripsi, peran (seperti admin atau pengguna biasa), dan atribut lain terkait pengguna.
- **Contoh Isi**:
```sql
username,password,role
john,hashed_password,USER
admin,hashed_password,ADMIN
```

3. **channel.csv**

- **Deskripsi**: File CSV yang menyimpan data tentang saluran atau channel yang telah dibuat dalam aplikasi. Setiap baris mungkin mencakup informasi seperti nama channel, daftar pengguna yang terhubung, dan mungkin kunci akses (jika ada fitur privasi saluran).
- **Contoh Isi**
```sql
channel_name,user_list,key
general,john,12345
public,alice,
```

4. **users.log**

- **Deskripsi**: File log yang berisi catatan aktivitas pengguna dalam aplikasi. Ini bisa mencakup kegiatan seperti login, logout, pembuatan atau penghapusan saluran, pengiriman pesan, dll. Tujuannya adalah untuk melacak jejak aktivitas pengguna untuk tujuan audit dan keamanan.
- **Contoh Isi**:
```csharp
[2024-06-28 13:45:23] User 'john' logged in.
[2024-06-28 14:02:11] User 'alice' created channel 'public'.
```

5. **auth.csv**

- **Deskripsi**: File CSV yang berisi data autentikasi untuk channel tertentu, seperti kunci atau password yang diperlukan untuk masuk ke saluran tertentu. Jika suatu channel memiliki fitur keamanan tambahan, pengguna harus memasukkan kunci yang benar untuk dapat mengakses saluran tersebut.
- **Contoh Isi**:
```sql
channel_name,auth_key
public,12345
private,54321
```

#### Cara Kerja

1. **Inisialisasi Server**:
Pada bagian awal program, server melakukan inisialisasi, seperti membuat soket (`socket()`), mengatur opsi soket (`setsockopt()`), mengikat soket ke alamat dan port tertentu (`bind()`), serta memulai mendengarkan koneksi dari klien (`listen()`).

2. **Koneksi dan Penanganan Klien**:
Server berjalan dalam loop tak terbatas (`while(1)`) untuk menerima koneksi dari klien menggunakan fungsi `accept()`. Setiap kali ada klien yang terhubung, server membuat thread baru untuk menangani klien tersebut.

3. **Thread Handler (`client_handler`)**:
Setiap thread yang baru dibuat menjalankan fungsi client_handler, yang bertanggung jawab untuk:

    - Menerima pesan dari klien menggunakan recv().
    - Memproses pesan yang diterima untuk mengekstrak perintah dan argumen.
    - Berdasarkan perintah yang diterima, menjalankan fungsi-fungsi yang sesuai untuk:
        - Autentikasi pengguna (LOGIN dan REGISTER).
        - Manajemen pengguna (seperti EDIT, REMOVE, LIST USER).
        - Manajemen channel (seperti CREATE CHANNEL, EDIT CHANNEL, DEL CHANNEL, LIST CHANNEL, JOIN, CREATE ROOM, EDIT ROOM, DEL ROOM, LIST ROOM, BAN, UNBAN).
        - Manajemen pesan (CHAT, SEE CHAT, EDIT CHAT, DEL CHAT).
        - Penanganan keluar (EXIT) dan lain-lain.
    - Mengirim balasan ke klien menggunakan send() untuk memberi tahu hasil dari operasi yang diminta.

### monitor.c

Program `monitor.c` berfungsi sebagai client untuk memonitor dan berinteraksi dengan server yang berjalan untuk aplikasi manajemen pesan real-time. Berikut adalah penjelasan lengkap tentang cara kerja program `monitor.c`:

- **Inisialisasi**: Program dimulai dengan memeriksa argumen dari baris perintah untuk memastikan format yang benar untuk login atau register.
- **Koneksi ke Server**: Membuat socket, mengatur alamat server (localhost dan port 8080), dan melakukan koneksi menggunakan `connect()`.
- **Autentikasi**: Mengirim perintah login atau register ke server dan menerima respons untuk menentukan apakah autentikasi berhasil.
- **Interaksi dengan Pengguna**: Setelah login berhasil, program memungkinkan pengguna untuk memasukkan perintah seperti `-channel` untuk memonitor chat di suatu channel dan ruang tertentu.
- **Monitor Chat**: Saat pengguna memilih untuk memonitor chat dalam suatu channel dan ruang, program membuat thread monitor yang memantau file chat terkait. Thread ini berjalan di latar belakang dan memeriksa perubahan pada file chat setiap detik.
- **Penghentian Program**: Pengguna dapat keluar dengan memasukkan perintah `EXIT`, yang akan menghentikan monitor chat dan menutup koneksi ke server sebelum program keluar.

### Fungsi-Fungsi yang Berhasil

1. REGISTER
2. LOGIN
3. CREATE CHANNEL
4. JOIN CHANNEL (NO KEY)
5. DEL CHANNEL
6. EDIT CHANNEL
7. LIST CHANNEL
8. CREATE ROOM
9. JOIN ROOM
10. DEL ROOM
11. LIST ROOM
12. CHAT
13. LIST USER
14. EDIT USERNAME
15. REMOVE USER
16. BAN USER
17. UNBAN USER
18. LOG ACTIVITY

### Fungsi-Fungsi yang Belum Berhasil

1. Daemonize
2. JOIN CHANNEL (WITH KEY)

# Penyelesaian Discorit (Setelah Revisi) 

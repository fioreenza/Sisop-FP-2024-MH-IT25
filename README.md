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

# Penyelesaian Discorit (Setelah Revisi) 

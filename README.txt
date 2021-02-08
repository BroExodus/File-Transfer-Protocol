Exercise 1 - Socket Programming
By Georgi Thomas

Creates a threaded server that allows user to upload files to the server. 

Client uses a Command Line Interface(CLI) to connect and download files to the server. The 
user must add files to send in the ClientFile directory. Files in the server can be found
in the ServerFile directory. The program compiles to TigerS for the server and TigerC for
the client. Do make -f Makefile to run the program and build before running. Open two 
terminal windows to run the program. On one run './TigerS', on the other run './TigerC'. 
Also, './TigerS' is ran on one terminal window, './bash.sh' can be ran on another terminal
to run 100 clients. 

1. Use tconnect <ip> <username> <password>. The ip should be the localhost which is
   '127.0.0.1'.
2. Use tget <filename> to download the file from the server to the client or tput <filename> to 
   upload the file from the client to the server.
3. Use exit to end the program.

------- Server Files: ------------------------
TigerS.c          -> Main File for the server
userDatabase.c    -> Database for the user/password, implements a dynamic array
userDatabse.h     -> Header file for database
userPass.txt      -> Text file containing user/pass, can add more user/pass to use them in the server
ServerFile        -> Directory that contains the files for the server


--------Client Files: ------------------------
lineParse.c       -> Main file for the client
ClientFile        -> Directory that contains the files for the server


--------Misc Files: --------------------------
Makefile          -> Build the program
fileCreator       -> Creates files to test the program with.
bash.sh           -> Runs 100 clients

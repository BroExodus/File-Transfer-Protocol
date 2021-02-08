#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h> 
  
int main(int argc, char* argv[]) {

    int live = 1;
    int ip;
    struct in_addr addr;
    char buf[INET_ADDRSTRLEN];
    char file_transfer[200];
    
   
    int connectBool = 0; /* 1 if connected, 0 if disconnected */
    while(live){
      while(!connectBool){
        //char connectCmd[100], ipCmd[100], userCmd[100], passCmd[100];
        char *connectCmd, *ipCmd, *userCmd, *passCmd;
        char *buff;
        static size_t currentLineSize = 1000;
        printf(">> Hello user! Please connect to the server using tconnect <ip> <user> <pass> \n");
        //char rtm[400];
        printf(">> ");
        getline(&buff, &currentLineSize, stdin);
        //printf("Buffer initially: %s \n", buff);
        
        connectCmd = strtok(buff, " ");
        ipCmd = strtok(NULL, " ");
        userCmd = strtok(NULL, " ");
        passCmd = strtok(NULL, " ");
        sscanf(passCmd, "%s", passCmd);
        
        
        //fflush(stdin); //Clear the stdin for error checking
        //fgets(rtm, 400, stdin);
        //scanf("%s %s %s %s", connectCmd, ipCmd, userCmd, passCmd); //alt way of doing
        int connCmdLen = 4;
        //printf("Full Command: %s \n", rtm); //Error checking
        
        //sscanf(boom, "%s %s %s %s", connectCmd, ipCmd, userCmd, passCmd);//Extract values
        printf("Connect arg: %s \nIP: '%s' \nUser: %s  \nPass: %s \n", connectCmd, ipCmd, userCmd, passCmd ); //Error checking
        if (connCmdLen == 4){
          if (!strncmp(connectCmd, "tconnect",99)){
            ip = inet_pton(AF_INET, ipCmd, &addr ); //Returns 1 if ip was valid, 0 else; Change to when actually connecting to server
            if (ip){
              inet_ntop(AF_INET, &addr, buf, sizeof(buf));
    		      printf(">> Connecting on '%s' with Username: '%s' and Password: '%s'\n",buf, userCmd, passCmd);
                    
              //Insert Client Connection here
              
              char message[2000]; //Message to send
              char buffer[2000]; //Message from the server
              int clientSocket;
              struct sockaddr_in serverAddr;
              
              socklen_t addr_size;
              
              // Create the socket. 
              clientSocket = socket(AF_INET, SOCK_STREAM, 0);
              
              // printf("Socket created! \n");
              //Configure settings of the server address
              // Address family is Internet 
              serverAddr.sin_family = AF_INET;
              
              //Set port number, using htons function 
              serverAddr.sin_port = htons(1025);
              
              //Set IP address to localhost
              serverAddr.sin_addr.s_addr = inet_addr(buf);
              memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
            
              //Connect the socket to the server using the address
              addr_size = sizeof serverAddr;
              
              //printf("Trying Connection. \n");
              int connectVerify = connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
              if(connectVerify == -1){
                printf(">> Invalid socket. Exiting. Please try again.\n");
                perror(">> The error is:");
                return 0;
              }
              //printf("Connection successful. \n"); //Error checking
              
              memset(message, 0, sizeof(message));
              strcpy(message, userCmd);
              strcat(message, " ");
              strcat(message, passCmd);
              strcat(message, "\n");
              if( send(clientSocket , message , strlen(message) , 0) < 0){
                printf(">> Send failed user/password\n");
              }
              memset(message, '\0', sizeof(message));
              
              //Read the message from the server into the buffer
              memset(buffer, 0, sizeof(buffer));
              if(recv(clientSocket, buffer, 2000, 0) < 0){
                 printf(">> Receive verify failed\n");
                 perror(">> The error was");
              }
              printf("%s\n", buffer);
              if(!strcmp(buffer, "valid")){
                //printf(">> Welcome to the system: %s\n", userCmd);
                memset(buffer, '\0', sizeof(buffer));
                connectBool = 1;
              } 
              else if(!strcmp(buffer, "invalid")){
                printf(">> Your password was incorrect. \n");
                memset(buffer, '\0', sizeof(buffer));
                connectBool = 0;
		            live = 0;
                return 0;
              }
              else if(!strcmp(buffer, "unvalid")){
                printf(">> Your username was invalid, it could not be found in the database. \n");
                memset(buffer, '\0', sizeof(buffer));
                connectBool = 0;
		            live = 0;
                return 0;
              }
              //Print the received message
              //printf("Data received: %s\n",buffer);
              
              
              
              while(connectBool){ 
                printf(">> Welcome to Tiger Drive %s! What command would you like: tget tput exit\n", userCmd);
                char dtm[200]; //Array for inputs
                char *buff = NULL;
                static size_t currentLineSize = 1000;
                char *accessCmd, *argCmd = NULL;
                //memset(stdin, '\0', sizeof(stdin));
                //memset(dtm, '\0', sizeof(dtm));
                printf(">> ");
                fflush(stdin); //Clear the stdin for error checking
                //fgets(dtm, strlen(dtm), stdin); //get the input
                
                getline(&buff, &currentLineSize, stdin);
                accessCmd = strtok(buff, " ");
                
                sscanf(accessCmd, "%s", accessCmd);//Remove the new line char/null
              
                argCmd = strtok(NULL, " "); 
              
                printf(">> The access command is: '%s' \n", accessCmd); //Error checking
                if(!(strcmp(accessCmd, "exit"))){
                  live = 0;
                  connectBool = 0;
                  printf("You have been disconnected. Good Day. \n");
                  send(clientSocket , accessCmd , strlen(accessCmd) , 0);
                  
                  return 0;
                }
                
                sscanf(argCmd, "%s", argCmd); //Remove the new line char/null
                
                //scanf("%s %s", accessCmd, argCmd); //alt way of doing
                int cmdLength = 2;
                
                //int cmdLength = sscanf(dtm, "%s %s", accessCmd, argCmd);
                //printf(">> Argument: '%s' \n", dtm); //Error checking
                printf(">> The access command is: '%s' \n", accessCmd); //Error checking
                printf(">> The argument command is: '%s' \n", argCmd); //Error checking
                //printf(">> The cmdLen is %d\n", cmdLength); //Error checking
                
                if (!((!strcmp(accessCmd, "tget")) || (!strcmp(accessCmd, "tput")))){
                  printf(">> Parameters wrong, use tget/tput [filename]. \n");
                }
                else{
                  if (cmdLength == 2){
                    memset(message, '\0', sizeof(message));
                    strncpy(message, accessCmd, 999);
                    strcat(message, " ");
                    strcat(message, argCmd);
                    //printf(">> Message is : %s \n", message);
                    
                    if (!strcmp(accessCmd, "tget")){
                      
                      if( send(clientSocket , message , strlen(message) , 0) < 0){
                        printf(">> Getting file command failed to send\n");
                      }
                      
                      memset(message, '\0', sizeof(message));
                      memset(buffer, '\0', sizeof(buffer));
                      if(recv(clientSocket, buffer, 1000, 0) < 0){
                         printf("Receive acknowledge failed\n");
                         perror("Error was");
                      }
                      if(!(strcmp(buffer, "Ack_tget"))){
                        printf(">> Server acknowledged tget. Getting file: %s\n", argCmd); //Ask for validity
                      }
                      memset(buffer, '\0', sizeof(buffer));
                      if(recv(clientSocket, buffer, 1000, 0) < 0){
                         printf("File exist check failed\n");
                         perror("Error was");
                      }

                      if(!(strcmp(buffer, "Error"))){
                        printf(">> Error getting file. File doesn't exist. \n");
                        memset(buffer, '\0', sizeof(buffer));
                      }
                      else{
                        FILE *fap;
                        memset(file_transfer, '\0', sizeof(file_transfer)); //Clear file_transfer buffer to allow values to send
                        strcpy(file_transfer, "ClientFile/");
                        strcat(file_transfer, argCmd);
                        fap = fopen(file_transfer, "w"); //Open file to write to
                        memset(file_transfer, '\0', sizeof(file_transfer)); //Clear file_transfer buffer to allow values to send
                        
                        char finish_message[100]; //Check for finished message
                        
                        while (1) {
                          if (recv(clientSocket, file_transfer, sizeof(file_transfer), 0) == -1){
                            printf(">> Connection was broken\n");
                            perror("Error was");
                            break;
                            
                          }
                          sscanf(file_transfer, "%s", finish_message);
                          if(!(strcmp(finish_message, "done_sending"))){
                            printf("Done recieving file \n");
                            break;
                          }
                          memset(finish_message, '\0', sizeof(finish_message)); //Clear buffer to send
                          
                          //printf("The iteration is: %d \n", printNum++);//Error checking
                          printf("%s", file_transfer);//Error checking
                          //fwrite(file_transfer, sizeof(char), sizeof(file_transfer), fp);
                          
                          int err = fputs(file_transfer, fap);
                          //int err = fputs("The test is go\n", fap);
                          if(err == EOF)
                            printf("Failed to write to the file\n");
                          //fprintf(fp, "%s", file_transfer);
                          memset(file_transfer, '\0', sizeof(file_transfer)); //Clear buffer to send
                        }
                        fclose(fap);
                        memset(buffer, '\0', sizeof(buffer)); //Clear buffer to send
                      }
                    }
                    
                    
                    
                    
                    else if (!strcmp(accessCmd, "tput")){
                      if( send(clientSocket , message , strlen(message) , 0) < 0){ //Send file comamnd
                        printf(">> Sending file command failed to send\n");
                      }
                      memset(buffer, '\0', sizeof(buffer));
                      if(recv(clientSocket, buffer, 2000, 0) < 0){ //Check to recieve the acknowledge
                         printf("Receive failed\n");
                      }
                      if(!(strcmp(buffer, "Ack_tput"))){
                        printf(">> Server acknowledged tput \n");
                        printf(">> Uploading file: %s\n", argCmd);
                        
                        memset(file_transfer, '\0', sizeof(file_transfer)); //Clear buffer to send
                        strcpy(file_transfer, "ClientFile/");
                        strcat(file_transfer, argCmd);
                        FILE *fp = fopen(file_transfer, "r");
                        memset(file_transfer, '\0', sizeof(file_transfer)); //Clear buffer to send

                        while(fgets(file_transfer, sizeof(file_transfer), fp) != NULL) {

                          if (send(clientSocket, file_transfer, sizeof(file_transfer), 0) == -1) 
                            perror("[-]Error in sending file.");
                          
                          memset(file_transfer, '\0', sizeof(file_transfer)); //Clear buffer to send
                        }
                        fclose(fp);
                        
                        strcpy(file_transfer, "done_sending");  //Send message to say you're done sending the message
                        if (send(clientSocket, file_transfer, sizeof(file_transfer), 0) == -1) {
                            perror("[-]Error in sending finish message.");
                        }
                        memset(file_transfer, '\0', sizeof(file_transfer)); //Clear buffer to send
                        
                      } //Check for valid file
                    }//else if (!strcmp(accessCmd, "tput"))
                  }//if(cmdLength == 2)
                  else {
                    printf(">> No parameter given! Use tget/tput [filename]. \n");
                  } //Else - no param given
                } //Some else steatement 
                 
              } //while(connectBool)
              close(clientSocket);      
              
            }
            else if (ip == 0){ //Connection unsuccesful
    		      printf(">> Invalid address\n");}		
    	      else {
    		      printf(">> An error has occurred. \n");
            }
          } //Check connection
        }//Check if valid connect input 
        else{
          printf(">> Not enough parameters. \n");}
      }
    
    } //while live
    return 0;
}

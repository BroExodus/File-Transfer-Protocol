//Implement a server here.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "userDatabase.h"

//char client_message[2000]; //Message from client
//char buffer[2000]; //Message to send
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
DArray DynamicArray;         /* Dynamic Array for Data */
 

void * socketThread(int clientSocket) {
      //int connectBool = 1;
      char client_message[2000]; //Message from client
      char buffer[2000]; //Message to send
      
      char file_transfer[200];
      int newSocket = clientSocket;
      pthread_mutex_lock(&lock);
      //int newSocket = *((int *)arg);
      char argument[100], file_name[100];
      char testUser[100], testPass[100]; //to verify user
      int printNum = 0; //Use for testing
      
      int check = recv(newSocket , client_message , 2000 , 0);
      if(check == -1){
        perror("Error was");
      }
      sscanf(client_message, "%s %s", testUser, testPass);
      sscanf(testPass, "%s", testPass);
      printf("User Information: '%s'\n", client_message);
    
      int SearchData = SearchDArray(&DynamicArray, testUser, testPass);
      memset(buffer, '\0', sizeof(buffer));
      if(SearchData == 1){
        strcpy(buffer, "valid"); //username and password match
      }
      else if(SearchData == 0){
        strcpy(buffer, "invalid"); //password incorrect
        printf("The password is incorrect.\n");
      }
      else{
        strcpy(buffer, "unvalid"); //Username is invalid
        printf("The username is invalid.\n");
      }
    
      // Send message to the client socket 
      //pthread_mutex_lock(&lock);
      send(newSocket,buffer,13,0);
      pthread_mutex_unlock( &lock);
      
      while(1){
        
        //Read the message from the server into the buffer
        memset(client_message, '\0', sizeof(client_message));
        //memset(buffer, 0, sizeof(buffer));
        if(recv(newSocket, client_message, 2000, 0) < 0){
           printf("Receive failed\n");
           perror("Error was");
        }
        
        sscanf(client_message, "%s %s", argument, file_name);
        
        memset(client_message, '\0', sizeof(client_message));
        if(!strcmp(argument, "exit")){
          pthread_mutex_lock(&lock);
          memset(argument, '\0', sizeof(argument));
          memset(buffer, '\0', sizeof(buffer));//Clear buffer to send
          printf("Ending socket. \n");
          printf("Exit socketThread: %s \n", testUser);
          close(newSocket);
          pthread_mutex_unlock( &lock);
          pthread_exit(NULL);
          break;
        }
        
        else if(!strcmp(argument, "tget")){
          pthread_mutex_lock(&lock);
          memset(argument, '\0', sizeof(argument));
          printf("tget data received: %s\n",file_name);
          memset(buffer, '\0', sizeof(buffer));
          strcpy(buffer, "Ack_tget");
          if( send(newSocket , buffer , strlen(buffer) , 0) < 0){
            printf("Sending tget ack failed to send\n");
            perror("The error was");
          }
          
          memset(file_transfer, '\0', sizeof(file_transfer)); //Clear file_transfer buffer to allow values to send
          strcpy(file_transfer, "ServerFile/");
          strcat(file_transfer, file_name);
          FILE *fp = fopen(file_transfer, "r");
          if(fp == NULL){
            strcpy(buffer, "Error");  //Send message to say you're done sending the message
            if (send(newSocket, buffer, sizeof(file_transfer), 0) == -1) {
                perror("[-]Error in sending finish message.");
            }
            memset(buffer, '\0', sizeof(buffer)); //Clear buffer to send
          }
          else{           
            strcpy(buffer, "NoProb");  //Send message to say you're done sending the message
            if (send(newSocket, buffer, sizeof(file_transfer), 0) == -1) {
                perror("[-]Error in sending finish message.");
            }
            memset(buffer, '\0', sizeof(buffer)); //Clear buffer to send
            memset(file_transfer, '\0', sizeof(file_transfer)); //Clear buffer to send
  
            while(fgets(file_transfer, sizeof(file_transfer), fp) != NULL) {
  
              if (send(newSocket, file_transfer, sizeof(file_transfer), 0) == -1) 
                perror("[-]Error in sending file.");
              
              memset(file_transfer, '\0', sizeof(file_transfer)); //Clear buffer to send
            }
            fclose(fp);
            
            strcpy(buffer, "done_sending");  //Send message to say you're done sending the message
            if (send(newSocket, buffer, sizeof(file_transfer), 0) == -1) {
                perror("[-]Error in sending finish message.");
            }
            memset(file_transfer, '\0', sizeof(file_transfer)); //Clear buffer to send  
            memset(buffer, '\0', sizeof(buffer)); //Clear buffer to send
            
          }
          pthread_mutex_unlock( &lock);
          sleep(1);
          
        }
        
        else if(!strcmp(argument, "tput")){
          pthread_mutex_lock(&lock);
          memset(argument, '\0', sizeof(argument));
          printf("tput data received: %s\n",file_name);
          memset(buffer, '\0', sizeof(buffer));
          strcpy(buffer, "Ack_tput");
          if( send(newSocket , buffer , strlen(buffer) , 0) < 0){
            printf("Sending tput ack failed to send\n");
            perror("The error was");
          }
          
          FILE *fap;
          memset(file_transfer, '\0', sizeof(file_transfer)); //Clear file_transfer buffer to allow values to send
          strcpy(file_transfer, "ServerFile/");
          strcat(file_transfer, file_name);
          fap = fopen(file_transfer, "w"); //Open file to write to
          memset(file_transfer, '\0', sizeof(file_transfer)); //Clear file_transfer buffer to allow values to send
          
          char finish_message[100]; //Check for finished message
          
          while (1) {
            if (recv(newSocket, file_transfer, sizeof(file_transfer), 0) == -1){
              break;
              printf("Connection was broken");
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
          pthread_mutex_unlock( &lock);
          sleep(1);
        }
        else {
          printf("Something error occured, possible client closed. Possible message : '%s' '%s' \n",argument, client_message);
          close(newSocket);
          pthread_mutex_unlock( &lock);
          pthread_exit(NULL);
          break;
        }
      }
    
    
    //printf("Exit socketThread \n");
    //close(newSocket);
    //pthread_exit(NULL);
}

void createStruct(DArray *ArrayPointer){
    //use files
    
    CreateDArray(ArrayPointer, 100); //Initialize the array
    Data TempData;             /* temp variable to hold data   */
    
    char user[100], pass[100];
    
    FILE *fp = fopen("userPass.txt","r"); //Open file
  
    // Return if could not open file 
    if (fp == NULL) 
      printf("Error in opening file\n");
    else{
      int i = getc(fp);
      do{ 
          fscanf(fp, "%s", user); //Get the username from file
          //printf("Username: %s     ", user);
          
          fscanf(fp, "%s", pass);
          //printf("Password: %s\n", pass);
           
          
          strncpy(TempData.User, user,99); //Store the username in data 
          strncpy(TempData.Pass, pass,99); //Store the password in data
          
          PushToDArray(ArrayPointer, &TempData); //Store the data in the array
          i = getc(fp);
      }  while(i!= EOF); 
   
    fclose(fp); 
    }
}




int main(){
    createStruct(&DynamicArray);
    
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    
    //Create the socket. 
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    // Configure settings of the server address struct
    
    // Address family = Internet 
    serverAddr.sin_family = AF_INET;
    
    //Set port number, using htons function to use proper byte order 
    serverAddr.sin_port = htons(1025);
    
    //Set IP address to localhost 
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //Set all bits of the padding field to 0 
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    
    //Bind the address struct to the socket 
    bind(serverSocket, (struct sockaddr *)  &serverAddr, sizeof(serverAddr));
    
    
    
    
    //Listen on the socket, with 100 max connection requests queued 
    if(listen(serverSocket,100)==0){
      printf("Listening\n"); }
    else {
      perror("Error in listen was");
    }
    pid_t pid[101];
    int i = 0;
    
    while(1){
      /*---- Accept call creates a new socket for the incoming connection ----*/
      addr_size = sizeof serverStorage;
      newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
      int pid_c = 0;

      if ((pid_c = fork())==0){
          socketThread(newSocket);
      }
      else{
        pid[i++] = pid_c;
        if( i >= 100){
           i = 0;
           while(i <100)
              waitpid(pid[i++], NULL, 0);
           i = 0;
        }
      }
    }
  
  return 0;
}

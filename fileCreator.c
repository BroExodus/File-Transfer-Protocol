//Make files here
//upload(n).txt
//down(n).txt
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

int main(){
    FILE *fap;
    char up[100], dwn[100];

    char textUp[1000];
    strcpy(textUp, "The text shown is here is for uploading. The data should look exactly like this. \n 1234567890 \n The fat cat sat on the big mat.\n");
    
    
      strcpy(up, "ClientFile/Test.txt");
      fap = fopen(up, "w"); //Open file to write to
      if(fap == NULL)
        perror("Error is");
      fputs(textUp, fap);
      fclose(fap);

  return 0;          
}
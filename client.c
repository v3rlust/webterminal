#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 4444
#define PresharedEncryptionKey 99 ;
//As all we know TCP connection is Vulnerable and when we are talking about authentification we need to talk a little bit about security
// so this is an Xor encryption to avoid sending clear information because of TCP protocol even this algorithme is weak and will be broken is 
//but we implement this encryption to add a little obfuscation for the informations send it especially Passwords
char * Enryption(char * EncMsg) 
{
    int length ;
    length=strlen(EncMsg);
    for (int i=0;i<length;i++)
        EncMsg[i]=EncMsg[i]^PresharedEncryptionKey ;

    return EncMsg ;
}

int main(){

    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    char buffer[128];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        printf("Connection error.\n");
        exit(1);
    }
    printf("Socket created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; //Internet Domaine
    serverAddr.sin_port = htons(PORT); //Server Port
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //server IP

    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); //start connection to server 
    if(ret < 0){
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Connected to Server.\n");
    printf("Please Send Login and password in format (Login:password) \n");
    scanf("%s", &buffer[0]);
    strcpy(buffer,Enryption(buffer)) ;
    printf("Encryted Data : ");    
    printf("%s \n",buffer);
    write(clientSocket, buffer, strlen(buffer));  //Send Credential to server
    if(read(clientSocket, buffer, 128) < 0) //Read Msg from server 
    {
        printf("data error Receiving \n");
    }
    else{
        printf("Server:%s \n", buffer);
        if (strstr(buffer,"Sorry")) //I send from server a message Start by Sorry so if This msg received by client the server socket was closed and now we will close client socket and exit the server site
        {
            close (clientSocket) ;
            exit(0) ;
        }

        else

        {

            while (1) 
        {


            if(read(clientSocket, buffer, 128) < 0) //Read Msg from server 
               {
                      printf("data error Receiving \n");
               }
            printf("Server;%s\n",buffer);

            //Here if Connection was Accepted we start to read and create actions.



        }

        }

        }
    

    return 0;
}

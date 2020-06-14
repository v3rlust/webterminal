#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT_SERVEUR 4444
#define MAXCHARS 8192 
#define MAXCHARSSEND 128


/*In this function we load the whole text file in Memory after that we try to find the credential send by strstr (substring in string) */
int SearchFile(char* Lpwd) 
{
	char Lpwd1 [128] ;
	int Checker ; 
	int n;
	char FileBuffer[MAXCHARS];
	int fd1 ;
	fd1=open("Serverpassword.txt",O_RDONLY);
	if(-1==fd1)
	{
		perror("open");
     exit (EXIT_FAILURE);  /* Test of the opeining of file */
	}
	n=read(fd1,FileBuffer, MAXCHARS);
  FileBuffer[n-1]='\0';  /*add null caractere to close the string*/
	close(fd1);

	strcpy(Lpwd1,Lpwd);
	char ch ='\n' ; 
  strncat(Lpwd1,&ch,1); // We need to add /n to avoid authentication error like if we send just a part of the password and not all the password and as we know 
  //in the file every credential was seperated by new line that's why we are searching with xxxx\n however the last line in file must have \n
  if (strstr(FileBuffer,Lpwd1)==NULL) 
  {
  	Checker = 1 ; 

  }
  else 
  {
  	Checker=0 ; 
  }

  return Checker ; //return the value of chaeck
}



int main(int argc, char **argv)
{

	char ReceivedMsg [MAXCHARSSEND] ;
	char SendResponse [MAXCHARSSEND] ;
	int lengthReceivedMsg ;
	int Check ;
	struct sockaddr_in SIN ; 
	int socket_ecoute,socket_service ; 
	int socketlen ; 
	socket_ecoute=socket(PF_INET,SOCK_STREAM,0); //Create new Socket to start listening on specific port

	if(socket_ecoute==-1) 
	{
		perror("sock") ; 
		exit(2) ; //Check if the socket was create succesffuly or not
	}
	memset(&SIN,0,sizeof(SIN)); // Initialise the struct to 0
	SIN.sin_family=AF_INET; //Domaine Internet
	SIN.sin_port=htons(PORT_SERVEUR); //Server Port 
	SIN.sin_addr.s_addr=INADDR_ANY; //

	if((bind(socket_ecoute,(struct sockaddr*)&SIN,sizeof(SIN)))!=0)   //Bind the socket Network Interface
	{
		printf("socket bind failed...\n"); //Test bind
		exit(4); 
	}

	if(!listen(socket_ecoute,10)) //start listening with 10 connection maximum
	{
		while(1)  //start infinity loop in order to make the server always searching for new connection and handle all socket received in the queu
		{
			socket_service=accept(socket_ecoute,(struct sockaddr*)&SIN,&socketlen); //Accpet connection with connection demande and attach it to an anther service socket
			if(socket_service==-1)
			{
				perror ("accept !") ;  //test Accept function 
				exit(EXIT_FAILURE) ; 
			}

			if (fork()==0)  //In order to handle multi client we need to create fils to every client want to connect and this fils will be handling all the work of the client
			{
				close(socket_ecoute); //close listin socket we don't need anymore


				lengthReceivedMsg=read(socket_service, ReceivedMsg, 1024); //Read 1024 from the Socket and copy it to ReceivedMsg
				switch (lengthReceivedMsg)  //Testing the received data
				{
					case -1:  
					perror("Error receiving msg \n");   
					close(socket_service); 
					case 0: 
					fprintf(stderr,"Connection lost with client"); 
					close(socket_service); 
					

					default: 

			               Check=SearchFile(ReceivedMsg); // Check if login credential exist or no 
			               if (Check==0)
			               {

			                  memset(SendResponse,'\0',sizeof(SendResponse)); //set SendResponse to null string
			                  memcpy(SendResponse,"Successfull Login \n Connection Istablish Successfully \n",MAXCHARSSEND) ; //copy message to SendResponse value
			                  write (socket_service,SendResponse,MAXCHARSSEND) ; //Send Response to the client 

			                  while (1)
			                  {


			                  	//Here we will hold the connection after connection establishing


			                  }
			              }

			              else
			              {
			              	memset(SendResponse,'\0',sizeof(SendResponse));
			              	memcpy(SendResponse,"Sorry Login incorrect \n Good Bye Server Disconnected \n",MAXCHARSSEND) ;
			              	write (socket_service,SendResponse,MAXCHARSSEND	) ;
			              	close (socket_service) ;
			                  //exit(10);
			              }
			          }

			          close(socket_service) ;
			          
			          


			      }
			  }

			}

			else 
			{

		printf("Listen failed...\n");  //test of listing
		exit(3); 

	}

	return 0;
}   

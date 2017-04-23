#include <stdio.h>
#include <string.h>    
#include <stdlib.h>    
#include  <sys/socket.h>
#include <netinet/in.h> 
#include <pthread.h>
 
#define ERROR_CONNEXION  1
FILE *f;
int c0=30;
pthread_mutex_t S = PTHREAD_MUTEX_INITIALIZER ;


void *executerCommande(void *socker_clt)
{
    int sock = *(int*)socker_clt;
    int read_size;
    char *message , client_message[2000];

    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
	int n,c,pos ; 
	char chn[30],chc[30],*p,*p2;

	p=strchr(client_message,'/');
	pos=p-client_message+1;

	strncpy(chn,client_message,pos-1);
	chn[pos-1]='\0';

	p2=strchr(p+1,'/');
	pos=p2-p-1;

	strncpy(chc,p+1,pos);
	chc[pos]='\0';	

	c=atoi(chc);
	char ch[200];

	pthread_mutex_lock(&S);
	if (c0-c>=0) 
	{
    		c0-=c;
		f=fopen("historique.txt","a");
		fprintf (f,"%s %s Inscrit : capacite restante = %d\n",chn,chc,c0);
		sprintf (ch," Inscrit : capacite restante =  %d",c0);
		strcat(client_message,ch);
        	write(sock ,ch , strlen(ch));
		strcpy(client_message,"");
		fclose(f);
	}
	else
	{
		 
		f=fopen("historique.txt","a");
		fprintf (f,"%s %s Non inscrit : capacite restant = %d\n",chn,chc,c0);
		sprintf (ch," Non inscrit : capacite restant = %d",c0);
		strcat(client_message,ch);
        	write(sock , ch , strlen(client_message));
		strcpy(client_message,"");
		fclose(f);
		
	}	
	pthread_mutex_unlock(&S);	
   


    }
     
    if(read_size == 0)
    {
        puts("Client disconected");
    }
    else if(read_size == -1)
    {
        perror("recv failure");
    }
         
    free(socker_clt);
     
    return 0;
}

 
int main(int argc , char *argv[])
{
    int socker_serv , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    socker_serv = socket(AF_INET , SOCK_STREAM , 0);
    if (socker_serv == -1)
    {
        printf("Socket Fail");
    }
    puts("Socket created");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
   //server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons( 8888 );

    if( bind(socker_serv,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror(" error : fail bind!!");
        return ERROR_CONNEXION;
    }
    puts("bind succeed	");

    listen(socker_serv , 3);
    puts("waiting Client  ...........");
    c = sizeof(struct sockaddr_in);
     
    		f=fopen("historique.txt","a");
		fprintf (f,"   *****************************\n");
		fclose(f);

    while( (client_sock = accept(socker_serv, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("CONNECTION ACCEPTED");
         
        pthread_t commandeThread;
        new_sock = malloc(sizeof(int));
        *new_sock = client_sock;  
        if( pthread_create( &commandeThread , NULL ,  executerCommande , (void*) new_sock) < 0)
        {
            perror("thread not created");
            return 1;
        }
         
    }
     
    if (client_sock < 0)
    {
        perror("FAILURE");
        return 1;
    }
     pthread_mutex_destroy(&S);
    return 0;
}
 
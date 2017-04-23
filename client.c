#include<stdio.h> 
#include<string.h>    
#include<sys/socket.h>    
#include<netinet/in.h>
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    puts("Bienvenue !! \n");
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("socket not created");
    }

    puts("Socket created ! ");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Connection fail");
        return 1;
    }
     
    puts("CONNECTED :\n");

    while(1)
    {
   	int n; 
   	int q ; 
	do{
	printf("Client number : ") ;
 	scanf("%d",&n); 
	}while (n<0);

	do{
	printf("Nombre de sportifs : ") ; 
	scanf("%d", &q) ; 
	}while (q<0);

	sprintf(message,"%d/%d/  ",n,q) ; 

        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("SEND FAILURE");
            return 1;
        }
         sleep(1);
        
	strcpy(server_reply,"");
	
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("RECEPTION FAILURE");
            break;
        }
         
        puts("SERVER MSG :");
        puts(server_reply);
    }
     
    close(sock);
    return 0;
}

#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

/* portul folosit */
#define PORT 2024

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void executie(void *);

int main ( )
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int sd;		//descriptorul de socket 
  int pid;
  pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
	int i=0;
  

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
    {
      int client;
      thData * td; //parametru functia executata de thread     
      int length = sizeof (from);

      printf ("[server_aditional]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}
	
        /* s-a realizat conexiunea, se astepta mesajul */
    
	// int idThread; //id-ul threadului
	// int cl; //descriptorul intors de accept

	td=(struct thData*)malloc(sizeof(struct thData));	
	td->idThread=i++;
	td->cl=client;

	pthread_create(&th[i], NULL, &treat, td);	      
				
	}   
};				
static void *treat(void * arg)
{		
		struct thData tdL; 
		tdL= *((struct thData*)arg);	
		printf ("[server_aditional_Thread %d ]- Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());		
		executie((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		
};
void executie(void *arg)
{
        int nr, i=0;
        char msg[10];		 
        char msg2[20];
        char username[20], parola[20];
        char rasp[10];
	struct thData tdL; 
	tdL= *((struct thData*)arg);
	bzero (msg, 10);
      
         //citim numarul aplicatiei
       if (read(tdL.cl, msg, 10) <= 0)
	{
	  perror ("[server]Eroare la read() de la client.\n");
	  printf("[Thread %d]\n",tdL.idThread);
	}
         printf("[server_aditional_Thread %d]Am primit mesajul de la [clientul_aditional]...%s\n", tdL.idThread, msg);
	//cream o noua conexiune spre server_2fa
	  int port;
         int sd1;			// descriptorul de socket
         struct sockaddr_in server1;
         struct sockaddr_in from1;

  /* stabilim portul */
  port = 2025;

  /* cream socketul */
  if ((sd1 = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
 //     return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server1.sin_family = AF_INET;
  /* adresa IP a serverului */
  server1.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* portul de conectare */
  server1.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd1, (struct sockaddr *) &server1,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
//      return errno;
    }


  //trimitem la server numarul aplicatiei
  if (write (sd1, msg, 1) <= 0)
    {
      perror ("[server_aditional]Eroare la write() spre server_2fa.\n");
    }
      printf("[server_aditional_Thread %d]Trimitem mesajul catre [server_2fa]...%s\n",tdL.idThread,msg);
      if(strcmp(msg, "4")!=0){
    //citim urmatoarea comanda
      bzero(msg2, 20);
       if (read(tdL.cl, msg2, 20) <= 0)
	{
	  perror ("[server_aditional]Eroare la read() de la client.\n");
	  printf("[Thread %d]\n",tdL.idThread);
	}
	//transmitem cate server 2fa cum dorim sa ne logam, sau daca vrem sa iesim
	 printf("[server_aditional_Thread %d]Am primit mesajul de la [clientul_aditional]...%s\n", tdL.idThread, msg2);
	  if (write (sd1, msg2, sizeof(msg2)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
 
    }
         printf("[server_aditional_Thread %d ]Trimitem mesajul catre [server_2fa]...%s\n",tdL.idThread,msg2);
               if(strcmp(msg2, "inregistrare")==0 || strcmp(msg2, "logare")==0|| strcmp(msg2, "logared")==0){
       //citim usernameul transmis de client
         if (read (tdL.cl, username, sizeof(username)) <= 0)
	{
	   perror ("[server_aditional]Eroare la read() de la client_aditional.\n");
	  printf("[Thread %d]\n",tdL.idThread);
	}
	printf("[server_aditional_Thread %d ]Am primit mesajul de la [clientul_aditional]...%s\n",tdL.idThread,username);
	//trimitem catre server_2fa username-ul
	  if (write (sd1, username, sizeof(username)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
    }
              printf("[server_aditional_Thread %d]Trimitem mesajul catre [server_2fa] username:...%s\n",tdL.idThread,username);
      
      //citim parola de la client
       if (read (tdL.cl, parola, sizeof(parola)) <= 0)
	{
	   perror ("[server_aditional]Eroare la read() de la client_aditional.\n");
	   printf("[Thread %d]\n",tdL.idThread);
	} 
	printf("[server_aditional_Thread %d]Am primit mesajul de la [clientul_aditional]...parola: %s\n",tdL.idThread,parola);
	//transmitem parola mai departe 
	  if (write (sd1, parola, sizeof(parola)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
    }

         printf("[server_aditional_Thread %d]Trimitem mesajul catre [server_2fa] parola...%s\n",tdL.idThread,parola);
      }
     //citim rasp - inregistare dai, nui
     //logare dal, nul
     bzero(rasp,10);
  if (read (sd1, rasp, 3) < 0)
    {
      perror ("[server_aditional]Eroare la read() de la server_2fa.\n");
    }
    printf("[server_aditional_Thread %d]Am primit mesajul de la [server_2fa]...%s\n",tdL.idThread,rasp);
    //transmitem rasp la client
    if (write (tdL.cl, rasp,3)<=0)
    {
      perror ("[server_aditional]Eroare la write() spre client_aditional.\n");
    }
     printf("[server_aditional_Thread %d]Trimitem mesajul catre [clientul_aditional]...%s\n",tdL.idThread, rasp);
    if(strcmp(rasp,"dal")==0){
    char msg3[20];
    bzero(msg3,20);
    //citim urmatoare comanda daca logarea a avut succes daca vrem sa ne autentificam prin cod sau prin notificare
     if (read (tdL.cl, msg3, 20) <= 0)
	{
	  perror ("[server_aditional]Eroare la read() de la client_aditional.\n");
	   printf("[Thread %d]\n",tdL.idThread);
	}
	    printf("[server_aditional_Thread %d]Am primit mesajul de la [clientul_aditional]...%s\n",tdL.idThread,msg3);
	    //transmitem mai departe
	if (write (sd1, msg3, sizeof(msg3)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
    }
        printf("[server_aditional_Thread %d]Trimitem mesajul catre [server_2fa]...%s\n",tdL.idThread,msg3);
     if(strcmp(msg3,"cod")==0){
         int msg4;
         //citim codul din client
         if (read (tdL.cl, &msg4, sizeof(msg4)) <= 0)
	{
	  perror ("[server_aditional]Eroare la read() de la client_aditional.\n");
           printf("[Thread %d]\n",tdL.idThread);
	}
	    printf("[server_aditional_Thread %d]Am primit mesajul de la [clientul_aditional]...%d\n",tdL.idThread,msg4);
	    //transmitem codul catre server_2fa
	    if (write (sd1, &msg4, sizeof(msg4)) <= 0)
     {
         perror ("[client]Eroare la write() spre server.\n");
    }
        printf("[server_aditional_Thread %d]Trimitem mesajul catre [server_2fa]...%d\n",tdL.idThread,msg4);
        char msg5[100];
        bzero(msg5,100);
        //citim raspunsul 
         if (read (sd1, msg5, 3) <= 0)
    {
     perror ("[server_aditional]Eroare la read() de la server_2fa.\n");
    }
   
     printf("[server_aditional_Thread %d]Am primit mesajul de la [server_2fa]...%s\n",tdL.idThread,msg5);
     //transmitem raspunsul catre client
    if (write (tdL.cl, msg5,3)<=0)
    {
         perror ("[server_aditional]Eroare la write() spre client_aditional.\n");
    }
    
     printf("[server_aditional]Trimitem mesajul catre [clientul_aditional]...%s\n",msg5);
     if(strcmp(msg5,"dac")==0 && strcmp (msg2,"logared")==0){
     char mesaj2[100];
        if (read (tdL.cl, mesaj2, sizeof(mesaj2)) <= 0)
	{
               perror ("[server_aditional]Eroare la read() de la client_aditional.\n");
          printf("[Thread %d]\n",tdL.idThread);
	}
	 printf("[server_aditional_Thread %d]Am primit mesajul de la [client_aditional]...%s\n",tdL.idThread,msg5);
	if(write(sd1,mesaj2,sizeof(mesaj2))<=0)
	{
             perror ("[server_aditional]Eroare la write() spre server_2fa.\n");
	}
	printf("[server_aditional_Thread %d]Trimitem mesajul catre [serverul_2fa]...%s\n", tdL.idThread,mesaj2);
	char buffer[1024];
	bzero(buffer,1024);
	while (1)
	 {
	if(read(sd1, buffer, sizeof(buffer))<= 0)
	  {
       perror ("[client]Eroare la read() de la server.\n");
    }
    printf("[server_aditional_Thread %d]Am primit mesajul de la [server_2fa] %s\n",tdL.idThread, buffer);
    
       if (write (tdL.cl, buffer, sizeof(buffer))<=0)
    {
        perror ("[server_aditional]Eroare la write() spre client_aditional.\n");
    }
    bzero(buffer,1024);
} 
    }
    }
    else
    {
    //conectarea prin notificare
    char msg6[100];
    bzero(msg6,100);
    //citim raspunsul din server_2fa
             if (read (sd1, msg6, 3) < 0)
    {
      perror ("[server_aditional]Eroare la read() de la server_2fa.\n");

    }
    printf("[server_aditional_Thread %d]Am primit mesajul de la [server_2fa]...%s\n",tdL.idThread,msg6);
  
    if (write (tdL.cl, msg6,3)<=0)
    {
      perror ("[server_aditional]Eroare la write() spre client_aditional.\n");
    }
     printf("[server_aditional_Thread %d]Trimitem mesajul catre [clientul_aditional]...%s\n",tdL.idThread,msg6);
          if(strcmp(msg6,"daf")==0 && strcmp (msg2,"logared")==0){ //daca e pozitiv si suntem conectati ca dezvoltator, primim din server_2fa codurile app si le transimitem mai departe
     char mesaj2[100];
        if (read (tdL.cl, mesaj2, sizeof(mesaj2)) <= 0)
	{
	  perror ("[server_aditional]Eroare la read() de la client_aditional.\n");
           printf("[Thread %d]\n",tdL.idThread);
	}
	if(write(sd1,mesaj2,sizeof(mesaj2))<=0)
	{
       perror ("[server_aditional]Eroare la write() spre server_2fa.\n");
	}
	printf("[server_aditional_Thread %d]Trimitem mesajul catre [serverul_2fa]...%s\n",tdL.idThread,mesaj2);
	char buffer[1024];
	bzero(buffer,1024);
	while (1)
	 {
	if(read(sd1, buffer, sizeof(buffer))<= 0)
	  {
      perror ("[server_aditional]Eroare la read() de la server_2fa.\n");
    }
    printf("[server_aditional_Thread %d]Am primit mesajul de la [server_2fa] %s\n",tdL.idThread, buffer);
    
       if (write (tdL.cl, buffer, sizeof(buffer))<=0)
    {
      perror ("[server_aditional]Eroare la write() spre client_aditional.\n");
    }
    bzero(buffer,1024);
} 
    }
    }
    }
}    
    close(sd1);
    }



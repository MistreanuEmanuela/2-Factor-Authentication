#include <sys/types.h>
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
#define PORT 2026

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void executie(void *);

int main ( int argc, char *argv[])
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

      printf ("[client_2fa]Asteptam la portul %d...\n",PORT);
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
		printf ("[client_2fa_Thread %d]- Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());		
		executie((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		
};
void executie(void *arg)
{
        struct thData tdL; 
	tdL= *((struct thData*)arg);
        fflush (stdout);
      fflush (stdout);
      char mesaj[20];
      bzero(mesaj,20);
      // citim mesajului 
      if (read (tdL.cl, mesaj, sizeof(mesaj)) <= 0)
	{
	  perror ("[client_2fa]Eroare la read() de la server_2fa.\n");
	  printf("[Thread %d]\n",tdL.idThread);
	}
       printf ("[client_2fa_Thread %d]Mesajul a fost receptionat...%s\n",tdL.idThread, mesaj);
       if(strcmp(mesaj,"cod")==0)
       {
       //trimitem o cerere
        if (write (tdL.cl, "request", 7) <= 0)
    {
      perror ("[client_2fa]Eroare la write() spre server_2fa.\n");
     printf("[Thread %d]\n",tdL.idThread);
    }
      printf("[client_2fa_Thread %d]Trimitem mesajul catre [server_2fa]...request\n",tdL.idThread );
      int msg;
      //citim codul primit
        if (read (tdL.cl, &msg, sizeof(msg)) <= 0)
	{
	  perror ("[client_2fa]Eroare la read() de la server_2fa.\n");
	 printf("[Thread %d]\n",tdL.idThread);
	}
       printf ("[client_2fa_Thread %d]Mesajul a fost receptionat...%d\n",tdL.idThread, msg);
       printf("\n");
       printf ("CODUL DUMNEAVOSTRA ESTE: %d\n" ,msg);
       printf("\n");
       }
        if(strcmp(mesaj,"notif")==0)
       {
         char rasp[100];
         printf("\n");
         printf("INCERCATI SA VA CONECTATI?\n");
         printf("1.DA                ");
         printf("2.NU");
         printf("\n");
         fflush (stdout);
         fscanf(stdin, "%s", rasp);
         //trannsmitem raspunsul
         if(strcmp(rasp,"1")==0){
            if (write (tdL.cl, "daf", 3) <= 0)
    {
      perror ("[client_2fa_Thread %d]Eroare la write() spre server_2fa.\n");
    printf("[Thread %d]\n",tdL.idThread);
     }
      printf("[client_2fa_Thread %d]Trimitem mesajul catre [server_2fa]...daf\n",tdL.idThread);
       }
       else
       {
       if (write (tdL.cl, "nuf", 3) <= 0)
    {
             perror ("[client]Eroare la write() spre server.\n");
             printf("[Thread %d]\n",tdL.idThread);
    }
      printf("[client_2fa_Thread %d]Trimitem mesajul catre [server_2fa]...nuf\n",tdL.idThread);
       }
       
       }
    }

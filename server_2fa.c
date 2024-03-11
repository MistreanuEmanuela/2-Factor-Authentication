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
#define PORT 2025

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void executie(void *);

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int sd1;		//descriptorul de socket 
  int pid;
  pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
	int i=0;
  

  /* crearea unui socket */
  if ((sd1 = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd1,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
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
  if (bind (sd1, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd1, 2) == -1)
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

      printf ("[server_2fa]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd1, (struct sockaddr *) &from, &length)) < 0)
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
		printf ("[server_2fa_Thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());		
		executie((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		
};
int inregistrare(char user[20], char par[20], FILE *f)
{ 
          char info[100], info1[100];
          int g=0;
          while(fscanf(f, "%s", info)!=EOF){
          if(strcmp(info, "username")==0)
          {
           fscanf(f, "%s", info1);
             if(strcmp(info1,user)==0)
              {
                return 0;
                g=1;
               }
              }
              }
              if(g==0){
 	      fseek(f, 0, SEEK_END);
	      fprintf(f, "username %s ", user); 
	      fseek(f, 0, SEEK_END);
	      fprintf(f, "parola %s\n", par);
	      fclose(f);
	      return 1; 
	  }
	  }
int cod(FILE *f)
  {       
          char info[20], nr[20];
          while(fscanf(f, "%s", info)!=EOF){
            strcpy(nr,info);
            } 
            int n;
            n=atoi(nr);
            n=n+1;
            char nr1 = (char) n;
    	   fseek(f, 0, SEEK_END);
	   fprintf(f, "%d\n", nr1);
	   return nr1;
	   fclose(f);
	  }
int logare(char *user, char *par, FILE *f)
{ 
          char info[20], info1[20];
          int g=0;
          fscanf(f, "%s", info); //citim primele 4 cuvinte- linia dezvoltatorului
          fscanf(f, "%s", info);
          fscanf(f, "%s", info);
          fscanf(f, "%s", info);
          while(fscanf(f, "%s", info)!=EOF){
          if(strcmp(info, "username")==0)
          {
           fscanf(f, "%s", info1);
           if(strcmp(info1,user)==0)
             { 
               fscanf(f, "%s", info1); //cuv: parola	  
	       fscanf(f, "%s" , info1);
	       if (strcmp(info1, par)==0)
	        return 1;
	        }
	        }
	        }
	        return 0;
	        }
int logared(char *user, char *par, FILE *f)
{ 
          char info[20], info1[20];
          int g=0;
           fscanf(f, "%s", info1);
           fscanf(f, "%s", info1); //username
            if(strcmp(info1,user)==0)
             { 
           fscanf(f, "%s", info);
           fscanf(f, "%s", info);
           if (strcmp(info1, par)==0)
	        return 1;
	        }
	        return 0;
	        }
           	        
int validare_cod(int a, int b)
  {
    if (a==b)
       return 1;
    else
       return 0;
    }	      
	   
void executie(void *arg)
{
        int nr, i=0;
        char msg[10];		 
        char username[20], parola[20];
        char rasp[10];
	struct thData tdL; 
	tdL= *((struct thData*)arg);
	      bzero (msg, 10);
      fflush (stdout);
      
      /* citirea mesajului */
      if (read (tdL.cl, msg, 10) <= 0)
	{
	  perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
	  printf("[Thread %d]\n",tdL.idThread);
	}
       printf ("[server_2fa_Thread %d]Mesajul a fost receptionat...%s\n",tdL.idThread, msg);
 //am citit numarul comenzii ce reprezinta aplicatia in car evrem sa ne conectam, asadar cream fisierele de care avem nevoie
 char fisier1[20],fisier2[20];
 if(strcmp(msg,"1")==0)
 {
     strcpy(fisier1,"app1.txt");
     strcpy(fisier2,"users_app1.txt");
     
     }
 if(strcmp(msg,"2")==0){
    strcpy(fisier1,"app2.txt");
     strcpy(fisier2,"users_app2.txt");
 }
 if(strcmp(msg,"3")==0){
     strcpy(fisier1,"app3.txt");
     strcpy(fisier2,"users_app3.txt");
     }
    
     FILE *f1=fopen(fisier2, "r+");
            if(f1==NULL)
            {
             printf("eroare");
             }
     FILE *f2=fopen(fisier1, "r+");
          if(f2==NULL)
            {
             printf("eroare");
             } 
     if(strcmp(msg, "4")!=0){                     
     char msg2[100];
     bzero(msg2,100);
            if (read (tdL.cl, msg2, 20)<= 0)
	{
	  perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
         printf("[Thread %d]\n",tdL.idThread);
	}
	printf ("[server_2fa_Thread %d]Mesajul a fost receptionat...%s\n",tdL.idThread, msg2);
	
     if(strcmp(msg2, "inregistrare")==0|| strcmp(msg2,"logare")==0 || strcmp(msg2,"logared")==0)
      {
      //citim username-ul
       if (read (tdL.cl, username, 20) <= 0)
	{
	  perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
          printf("[Thread %d]\n",tdL.idThread);
	}
	
	 printf ("[server_2fa_Thread %d]Mesajul a fost receptionat...username: %s\n",tdL.idThread,username);
	 //citim parola
	 if (read (tdL.cl, parola, 20) <= 0)
	{
	  perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
          printf("[Thread %d]\n",tdL.idThread);
	} 
	 printf ("[server_2fa_Thread %d]Mesajul a fost receptionat...parola: %s\n",tdL.idThread,parola);
	if(strcmp(msg2, "inregistrare")==0){ 
	 if(inregistrare(username,parola,f1)==1){ //daca inregistrarea a avut loc cu succes
	  if (write (tdL.cl, "dai", 3) <= 0){
	   
      perror ("[server_2fa]Eroare la write() spre server_aditiona;.\n");
      printf("[Thread %d]\n",tdL.idThread);
    }
     printf ("[server_2fa_Thread %d]Mesajul trimis catre [server_aditional] dai\n",tdL.idThread);
    }

      else
      {
       if (write (tdL.cl, "nui", 3) <= 0){ //daca inregistrarea a esuat
	   
      perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
      printf("[Thread %d]\n",tdL.idThread);
    }
      printf ("[server_2fa_Thread %d]Mesajul trimis catre [server_aditional] nui\n",tdL.idThread);
    }
    }
      if(strcmp(msg2, "logare")==0){  //daca incercam sa ne logam
	 if(logare(username,parola,f1)==1){ //daca utilizatorul exista in baza de date
	  if (write (tdL.cl, "dal", 3) <= 0){ //logarea a reusit
	   
         perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
      printf("[Thread %d]\n",tdL.idThread);
    }
       printf ("[server_2fa_Thread %d]Mesajul trimis catre [server_aditional] dal\n",tdL.idThread);
       char msg3[20];
      bzero(msg3,20);
       if (read (tdL.cl, msg3, 20) <= 0) //citim cum vrem sa ne autentificam
	{
	  perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
	  printf("[Thread %d]\n",tdL.idThread);
	}
	 printf ("[server_2fa_Thread %d]Mesajul a fost receptionat... %s\n",tdL.idThread, msg3);
	 //cream o conexiune cu clientul_2fa
	 int port;
          int sd2;			// descriptorul de socket
        struct sockaddr_in server2;	// structura folosita pentru conectare 
        struct sockaddr_in from2;

  /* stabilim portul */
  port = 2026;

  /* cream socketul */
  if ((sd2 = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server2.sin_family = AF_INET;
  /* adresa IP a serverului */
  server2.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* portul de conectare */
  server2.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd2, (struct sockaddr *) &server2,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
    }
    //transmitem clientului 2fa cum ne conectam
  if (write (sd2, msg3, sizeof(msg3)) <= 0) 
    {
      perror ("[server_2fa]Eroare la write() spre client_2fa.\n");
    }
    printf("[server_2fa_Thread %d] Trimitem mesajul catre [clientul_2fa] %s\n" ,tdL.idThread, msg3);
	 if(strcmp(msg3,"cod")==0) //daca e prin cod
	 {
	   cod(f2);
	   int n=cod(f2);
           int msg4;
           char msj[100];
	   bzero(msj,100);
	   //daca clientul_2fa are o cerere a codului
	   if (read (sd2, msj, sizeof(msj)) <= 0) 
	{
	  perror ("[server_2fa]Eroare la read() de la client_2fa.\n");
	  }
	 printf("[server_2fa_Thread %d]Am citit din [client_2fa]...%s\n",tdL.idThread, msj);
	if(strcmp(msj,"request")==0){
           if (write (sd2, &n, sizeof(n)) <= 0) //transmitem codul
    {
      perror ("[client]Eroare la write() spre server.\n");
    }
      printf("[server_2fa_Thread %d]Trimitem mesajul catre [client_2fa]...%d\n",tdL.idThread,n);
         if (read (tdL.cl, &msg4, sizeof(msg4)) <= 0) //citim codul din client
	{
	  perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
          printf("[Thread %d]\n",tdL.idThread);
	}
	    printf("[server_2fa_Thread %d]Am primit mesajul de la [server_aditional]...%d\n", tdL.idThread,msg4);
	    
	    if(validare_cod(n, msg4)==1){  
	       if (write (tdL.cl, "dac", 3) <= 0) //daca validarea a avut loc cu succes transmitem clientului un rasp pozitiv
    {
                perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
                printf("[Thread %d]\n",tdL.idThread);
    }
          printf("[server_2fa_Thread %d]Trimitem mesajul catre [server_aditional]...dac\n", tdL.idThread);
          }
	   else{ //altfel negativ
	     if (write (tdL.cl, "nuc", 3) <= 0)
    {
                  perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
                 printf("[Thread %d]\n",tdL.idThread);
    }
          printf("[server_2fa_Thread %d]Trimitem mesajul catre [server_aditional]...nuc\n", tdL.idThread);
    }
    }
    }
    if(strcmp(msg3,"notif")==0)
    {
      char rasps[100];
      bzero(rasps,100);
      //citim din clientul_2fa raspunsul si il transmitem mai departe
       if (read (sd2, rasps, sizeof(rasps)) < 0)
    {
      perror ("[server_2fa]Eroare la read() de la client_2fa.\n");
    }
    printf("[server_2fa_Thread %d]Am primit mesajul de la [clientul_2fa]...%s\n",tdL.idThread, rasps);
     if (write (tdL.cl, rasps, sizeof(rasps)) <= 0)
    {
                  perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
                   printf("[Thread %d]\n",tdL.idThread);
    }
          printf("[server_2fa_Thread %d]Trimitem mesajul catre [server_aditional]...%s\n",tdL.idThread, rasps);
    }
    close(sd2);
}


      else
      { //daca logarea nu a avut loc cu succes
       if (write (tdL.cl, "nul", 3) <= 0){
	   
      perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
      printf("[Thread %d]\n",tdL.idThread);
    }
        printf ("[server_2fa_Thread %d]Mesajul trimis catre [server_aditional] nul\n",tdL.idThread);

    }		
}				
          if(strcmp(msg2, "logared")==0){ //aceeasi pasi de logare ca si in cazul unui utilizator
	  if(logared(username,parola,f1)==1){
	  if (write (tdL.cl, "dal", 3) <= 0){
	   
      perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
      printf("[Thread %d]\n",tdL.idThread);
    }
        printf ("[server_2fa_Thread %d]Mesajul trimis catre [server_aditional] dal\n",tdL.idThread);

       char msg3[20];
      bzero(msg3,20);
       if (read (tdL.cl, msg3, 20) <= 0)
	{
	  perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
          printf("[Thread %d]\n",tdL.idThread);
	}
	 printf ("[server_2fa_Thread %d]Mesajul a fost receptionat... %s\n",tdL.idThread, msg3);
	 
	 int port;
          int sd2;			// descriptorul de socket
        struct sockaddr_in server2;	// structura folosita pentru conectare 
        struct sockaddr_in from2;


  /* stabilim portul */
  port = 2026;

  /* cream socketul */
  if ((sd2 = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
   //   return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server2.sin_family = AF_INET;
  /* adresa IP a serverului */
  server2.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* portul de conectare */
  server2.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd2, (struct sockaddr *) &server2,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
    }
    
  if (write (sd2, msg3, sizeof(msg3)) <= 0)
    {
      perror ("[server_2fa]Eroare la write() spre client_2fa.\n");
    }
      printf("[server_2fa_Thread %d]Trimitem mesajul catre [client_2fa]...%s\n",tdL.idThread, msg3);
      
	 if(strcmp(msg3,"cod")==0)
	 {
	   cod(f2);
	   int n=cod(f2);
	   char msj[100];
	   bzero(msj,100);
	   if (read (sd2, msj, sizeof(msj)) <= 0)
	{
	  perror ("[server_2fa]Eroare la read() de la client_2fa.\n");
	  }
	 printf("[server_2fa_Thread %d]Am citit din [client_2fa]...%s\n",tdL.idThread, msj);
	if(strcmp(msj,"request")==0){
           int msg4;
           if (write (sd2, &n, sizeof(n)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
    }
      printf("[server_2fa_Thread %d]Trimitem mesajul catre [client_2fa]...%d\n",tdL.idThread,n);
         if (read (tdL.cl, &msg4, sizeof(msg4)) <= 0)
	{
	  perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
	   printf("[Thread %d]\n",tdL.idThread);
	}
	 printf ("[server_2fa_Thread %d]Mesajul a fost receptionat... %d\n",tdL.idThread, msg4);
	 
	    
	    if(validare_cod(n, msg4)==1){  
	       if (write (tdL.cl, "dac", 3) <= 0)
    {
                perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
               printf("[Thread %d]\n",tdL.idThread);
    }
            printf("[server_2fa_Thread %d]Trimitem mesajul catre [server_aditional]...dac\n",tdL.idThread);
             char mesaj2[100];
                if (read (tdL.cl, mesaj2, 20) <= 0)
	{
	   perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
	   printf("[Thread %d]\n",tdL.idThread);
	}
	 printf ("[server_2fa_Thread %d]Mesajul a fost receptionat... %s\n",tdL.idThread, mesaj2);
	 
           FILE *f2=fopen(fisier1, "r+");
          if(f2==NULL)
            {
             printf("eroare");
             }    
         char data[1024];
         bzero(data,1024);
        while(fscanf(f2, "%s", data)!=EOF){ 
           if (write(tdL.cl, data, sizeof(data)) <=0) {
           perror("[-]Error in sending file.");
           printf("[Thread %d]\n",tdL.idThread);
         }
           printf("[server_2fa_Thread %d]Trimitem mesajul catre [server_aditional]...%s\n",tdL.idThread,data);
         bzero(data,1024);
  }
        fclose(f2);
          }
          
	   else{
	     if (write (tdL.cl, "nuc", 3) <= 0)
    {
                perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
               printf("[Thread %d]\n",tdL.idThread);
    }

          printf("[server_2fa_Thread %d]Trimitem mesajul catre [server_aditional]...nuc\n",tdL.idThread);
    }
    }
    }
    if(strcmp(msg3,"notif")==0)
    {
      char rasps[100];
      bzero(rasps,100);
       if (read (sd2, rasps, sizeof(rasps)) < 0)
    {
     perror ("[server_2fa]Eroare la read() de la client_2fa.\n");
    }

    printf("[server_2fa_Thread %d]Am primit mesajul de la [clientul_2fa]...%s\n",tdL.idThread, rasps);
     if (write (tdL.cl, rasps, sizeof(rasps)) <= 0)
    {
               perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
               printf("[Thread %d]\n",tdL.idThread);
    }
          printf("[server_2fa_Thread %d]Trimitem mesajul catre [server_aditional]...%s\n",tdL.idThread, rasps);
      if(strcmp(rasps,"daf")==0)
      {
           char mesaj2[100];
                if (read (tdL.cl, mesaj2, 20) <= 0)
	{
	  perror ("[server_2fa]Eroare la read() de la server_aditional.\n");
          printf("[Thread %d]\n",tdL.idThread);
	}
          printf("[server_2fa_Thread %d]Am primit mesajul de la [server_aditional]...%s\n",tdL.idThread, mesaj2);
           FILE *f2=fopen(fisier1, "r+");
          if(f2==NULL)
            {
             printf("eroare");
             }    
         char data[1024];
         bzero(data,1024);
        while(fscanf(f2, "%s", data)!=EOF){ 
           if (write(tdL.cl, data, sizeof(data)) <=0) {
           perror("[-]Error in sending file.");
           printf("[Thread %d]\n",tdL.idThread);
         }
         printf("[server_2fa_Thread %d]Trimitem mesajul catre [server_aditional]...%s\n",tdL.idThread,data);
         bzero(data,1024);
  }
        fclose(f2);
      }   
    }
    close(sd2);
}


      else
      {
       if (write (tdL.cl, "nul", 3) <= 0){
	   
      perror ("[server_2fa]Eroare la write() spre server_aditional.\n");
      printf("[Thread %d]\n",tdL.idThread);
    }
         printf("[server_2fa_Thread %d]Trimitem mesajul catre [server_aditional]...nul\n",tdL.idThread);

    }		
}
        }
        }
	}

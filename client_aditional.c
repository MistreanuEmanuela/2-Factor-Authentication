#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100], msg2[1];   
  char username[20], parola[20];		

  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = 2024;

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

 //Citim aplicatia in care vrem sa ne conectam
  bzero (msg, 1);
  printf ("Alegeti aplicatia in care doriti sa va conectati:\n");
  printf ("1.APP1\n");
  printf ("2.APP2\n");
  printf ("3.APP3\n");
  printf("4.EXIT\n");
  fflush (stdout);
  fscanf(stdin, "%s", msg);
  if(strcmp(msg,"4")==0)
  {
   if(write (sd, msg, 1) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
   }
   else{
  //transmitem catre server numarul comenzii corespunzator aplicatiei in care dorim sa ne conectam
  if (write (sd, msg, 1) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
   
//alegem urmatoarea comanda
  bzero (msg2, 1);
  printf ("Alegeti comanda:\n");
  printf ("1.Inregistare\n");
  printf ("2.Autentificare ca utilizator\n");
  printf ("3.Autentificare ca dezvoltator\n");
  printf ("4.EXIT\n");
  fflush (stdout);
  fscanf(stdin, "%s", msg2);
  if(strcmp(msg2,"4")==0)
{ 
  //transmitem serverului faptul ca vrem sa iesim
  if (write (sd,"exit",5) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    }
  //Daca alegem sa ne inregistram 
if(strcmp(msg2,"1")==0)
{ 
  //transmitem serverului faptul ca vrem sa ne inregistram
  if (write (sd,"inregistrare", 13) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
     //transmitem username-ul citit de la tastatura
      bzero (username, 20);
      printf ("Introduceti un username: \n");
      fflush (stdout);
       fscanf(stdin, "%s", username);
       if (write (sd, username, sizeof(username)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
     //transmitem parola citita de la tastatura
    bzero (parola, 20);
     printf ("Introduceti o parola: \n");
     fflush (stdout);
       fscanf(stdin, "%s", parola);
       if (write (sd, parola, 20) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno; 
    }
    char rasp[10];
    bzero(rasp,10);
    //citim daca am rasp de la server, daca am reusit sau nu sa ne inregistram
    if (read (sd, rasp, sizeof(rasp)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    if(strcmp(rasp, "dai")==0){
    printf ("[server]Inregistrarea a avut loc cu succes\n");
    }
    else
    {
    printf("[server]Inregistrarea a esuat\n");
    }
    }
    //daca alegem sa ne logam ca utilizatori
 if(strcmp(msg2,"2")==0)
 {
 //transmitem serverului ca vrem sa ne logam
     if (write (sd,"logare", 6) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    
     //transmitem username-ul citit de la tastatura
      bzero (username, 20);
      printf ("Introduceti un username: \n");
      fflush (stdout);
       fscanf(stdin, "%s", username);
       if (write (sd, username, sizeof(username)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
     //transmitem parola citita de la tastatura
    bzero (parola, 20);
     printf ("Introduceti o parola: \n");
     fflush (stdout);
       fscanf(stdin, "%s", parola);
       
    if (write (sd, parola, sizeof(parola)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno; 
    }
    char rasp[10];
    bzero(rasp,10);
    //citim raspunsul
    if (read (sd, rasp, sizeof(rasp)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    //daca a avut loc continuam cu celelate comenzi
    if(strcmp(rasp, "dal")==0){
    printf ("Logarea a avut loc cu succes\n");
     char msg3[10];
    bzero (msg3, 10);
    //citim modul in care dorim sa ne autentificam
  printf ("Alegeti comanda:\n");
  printf ("1.Autentificare prin cod\n");
  printf ("2.Autentificare prin notificare\n");
  printf("3.EXIT\n");
  fflush (stdout);
  fscanf(stdin, "%s", msg3);
  if(strcmp(msg3,"3")==0)
  {
   if (write (sd, "exit", 5) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
   }
   else{
  //transmitem catre server 
  if(strcmp(msg3, "1")==0){
   if (write (sd, "cod", 3) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
     printf("INTRODUCETI CODUL PE CARE L-ATI PRIMIT IN APLICATIE\n");
     int cod;
     fflush (stdout);
     fscanf(stdin, "%d", &cod);
      if (write (sd, &cod, sizeof(cod)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    char rasp2[100];
    bzero(rasp2,100);
     if (read (sd, rasp2, sizeof(rasp2)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    if(strcmp(rasp2,"dac")==0)
    {
      printf("V-ati autentificat cu succes\n");
    }
    else
    {
    printf("Autentificarea a esuat\n");
    }
    }
  else
  {
  char rasp3[100];
  bzero(rasp3,100);
    if (write (sd, "notif", 5) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    if (read (sd, rasp3, sizeof(rasp3)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    if(strcmp(rasp3,"daf")==0)
    {
     printf("V-ati autentificat cu succes\n");
     }
     else{
     printf("Autentificarea a esuat\n");
    } 
  }
    }
    }
    else
    {
    //daca nu programul se opreste
    printf("[server]logarea a esuat\n");
    }
    }
    if(strcmp(msg2,"3")==0)
 {
 //transmitem serverului ca vrem sa ne logam ca dezvoltatori 
     if (write (sd,"logared", 7) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    
     //transmitem username-ul citit de la tastatura
      bzero (username, 20);
      printf ("Introduceti un username: \n");
      fflush (stdout);
       fscanf(stdin, "%s", username);
       if (write (sd, username, sizeof(username)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
     //transmitem parola citita de la tastatura
    bzero (parola, 20);
     printf ("Introduceti o parola: \n");
     fflush (stdout);
       fscanf(stdin, "%s", parola);
       
   
    if (write (sd, parola, sizeof(parola)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno; 
    }
    char rasp[10];
    bzero(rasp,3);
    //citim raspunsul
    if (read (sd, rasp, sizeof(rasp)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    //daca a avut loc continuam cu celelate comenzi
    if(strcmp(rasp, "dal")==0){
    printf ("[server]Logarea a avut loc cu succes\n");
     char msg3[10];
    bzero (msg3, 10);
    //citim modul in care dorim sa ne autentificam
  printf ("Alegeti comanda:\n");
  printf ("1.Autentificare prin cod\n");
  printf ("2.Autentificare prin notificare\n");
  printf("3.EXIT\n");
  fflush (stdout);
  fscanf(stdin, "%s", msg3);
    if(strcmp(msg3,"3")==0)
  {
   if (write (sd, "exit", 5) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
   }
  //transmitem catre server 
  else{
  if(strcmp(msg3, "1")==0){
   if (write (sd, "cod", 3) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
     printf("INTRODUCETI CODUL PE CARE L-ATI PRIMIT IN APLICATIE\n");
     int cod;
     fflush (stdout);
     fscanf(stdin, "%d", &cod);
      if (write (sd, &cod, sizeof(cod)) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    char rasp2[100];
    bzero(rasp2,100);
     if (read (sd, rasp2, sizeof(rasp2)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    //daca ne am logat cu succes
    if(strcmp(rasp2,"dac")==0)
    {
    char mesaj[100];
      printf("V-ati autentificat cu succes\n");
      printf("1.Coduri aplicatie\n");
      fflush (stdout);
      fscanf(stdin, "%s", mesaj);
      if(strcmp(mesaj,"1")==0){
       if(write(sd, "coduri",6)<=0)
       {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
     	char buffer[1024];
	bzero(buffer,1024);
	while (1)
	 {
	if(read(sd, buffer, sizeof(buffer))<= 0)
	  {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    printf("%s\n", buffer);
    bzero(buffer,1024);
    } 
    }
    }
    else
    {
    printf("Autentificarea a esuat\n");
    }
    }
  else
  {
  char rasp3[100];
  bzero(rasp3,100);
    if (write (sd, "notif", 5) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
    if (read (sd, rasp3, sizeof(rasp3)) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    if(strcmp(rasp3,"daf")==0)
    {
     printf("V-ati autentificat cu succes\n");
        char mesaj[100];
      printf("1.Coduri aplicatie\n");
      fflush (stdout);
      fscanf(stdin, "%s", mesaj);
      if(strcmp(mesaj,"1")==0){
       if(write(sd, "coduri",6)<=0)
       {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
     	char buffer[1024];
	bzero(buffer,1024);
	while (1)
	 {
	if(read(sd, buffer, sizeof(buffer))<= 0)
	  {
         perror ("[client]Eroare la read() de la server.\n");
     // return errno;
    }
    printf("%s\n", buffer);
    bzero(buffer,1024);
    } 
    }
     }
     else{
     printf("Autentificarea a esuat\n");
     }
  }
  }
    }
    else
    {
    //daca nu programul se opreste
    printf("[server]logarea a esuat\n");
    }
    }
    }
    close(sd);
    }
    

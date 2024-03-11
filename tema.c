#include<stdio.h>
#include<stdlib.h>
#include<<sys/wait.h>
#include<sys.types.h>
#include<sys.socket.h>
#include<unistd.h>
#include<string.h>
#include<sys/uio.h>
#include<utmp.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#define READ 0
#define WRITE 1
int l=0;
int main(void)
{
char comanda[30], username[50]‎;
printf("Introduceti comanda: ");
fscanf(stdin, "%s", comanda);
if(strcmp(comanda, "login")==0)
{
 if(l==0){ 
  int lungime=strlen("introduceti username");
  fprintf(stdout, %d: introduceti username\n", lungime);
  fscanf(stdin, "%s", username);
void process_parent(int read_fd, int write_fd)
{
  write(write_fd,username,30);
  char buff[30];
  int len_read=read(read_fd,buff,30);
  buff[len_read]='\0';
  if(strcmp(buff,"yes")==0)
{
 int lungime=strlen("login succeded");
 printf("%d: login succeded\n", lungime)
 l=1;
 main();
}
 if(strcmp(buff,"no")==0)
{
  int lungime=strlen("login failed");
  printf("%d: login failed\n", lungime);
  main();
}
}
 
void process_child(int read_fd, int write_fd)
{
 char buff[30];
 int len_read=read(read_fd, buff,100);
 buff[len_read]='\0';
 FILE *input=fopen("login.txt","r");
 char content[100];
 while(fscanf(input, "%s", content)!=EOF)
{
 if(strcmp(content,buff)==0)
  write(write_fd,"yes",3);
}
 write(write_fd,"no",2);
}
   int parent_to_child[2];
   int child_to_parent[2];
   pipe(parent_to_child);
   pipe(child_to_parent);
   pid_t pid=fork();
         if(pid<0){
            printf("eroare fork\n");
		exit(0);
}
         if(pid>0){
           close(parent_to_child[READ]);
	   close(child_to_parent[WRITE]);
           process_parent(child_to_parent[READ], parent_to_child[WRITE]);
}
         if(pid==0)
{
         close(parent_to_child[WRITE]);
	 close(child_to_parent[READ]);
	 process_child(parent_to_child[READ], child_to_parent[WRITE]);
}
}
 else
  { 
    int lungime;
    lungime=strlen("You need to logout");
    printf("%d: Ypu need to logout\n", lungime);
    main();
}
}
else
{
 if(strcmp(comanda, "get-logged-users")==0)
  if(l==0){
  int lungime=strlen("YOU DO NOT HAVE ACCES TO THIS INFORMATIONS");
  printf("%d: YOU DO NOT HAVE ACCES TO THIS INFORMATIONS\n",lungime);
  main();
}
  else
   {
    struct utmp *getutent(void);
    int child;
    child=fork();
    int i=1;
    struct utmp *entry;
    while(entry=getutent()){
    i=1;
    if(i==1){
     if(child==0)
      {
       mknod("p2.txt", S_IFIFO| 0666,0);
       int fd=open("p2.txt", O_WRONLY);
       write(fd,entry->ut_host,100);
}

  else{
    int fd=open("p2.txt", O_RDONLY);
    char buff[1000];
    int len=read(fd,buff,1000);
    buff[len]='\0';
    printf("HOST: %s ", buff);
}
}
i=i+1;
if(i==2){
 if(child==0)
      {
       mknod("p2.txt", S_IFIFO| 0666,0);
       int fd=open("p2.txt", O_WRONLY);
       write(fd,entry->ut_user,100);
}

  else{
    int fd=open("p2.txt", O_RDONLY);
    char buff[1000];
    int len=read(fd,buff,1000);
    buff[len]='\0';
    printf("USER: %s ", buff);
}
}
i=i+1;
if(i==3){
 if(child==0)
      {
       char times[50];
       int time=entry->ut_tv.tv_usec;
       sprintf(times,"%d",time);
       mknod("p2.txt", S_IFIFO| 0666,0);
       int fd=open("p2.txt", O_WRONLY);
       write(fd,times,100);
}

  else{
    int fd=open("p2.txt", O_RDONLY);
    char buff[1000];
    int len=read(fd,buff,1000);
    buff[len]='\0';
    printf("TIME: %s\n", buff);
}
}
} 
main();
}
else
{
 if(strcmp(comanda, "get-proc-info")==0)
{
 if(l==0){
 int lungime=strlen("ACCES DENIED");
 printf("%d: ACCES DENIED\n",lungime);
 main();
}
 else
{
  int sockets1[2], sockets2[2],sockets3[3],sockets4[2], sockets5[2],child;
  socketpair(AF_UNIX,SOCK_STREAM,0,sockets1);
  socketpair(AF_UNIX,SOCK_STREAM,0,sockets2);
  socketpair(AF_UNIX,SOCK_STREAM,0,sockets3);
  socketpair(AF_UNIX,SOCK_STREAM,0,sockets4);
  socketpair(AF_UNIX,SOCK_STREAM,0,sockets5);
 char pid1[30];
 char info1[1024], info2[1024], info3[1024], info4[1024], info5[1024];
 int lungime=strlen("introduceti pid procces cautat");
 printf("%d: introduceti pid procces cautat\n", lungime);
 fscanf(stdin, "%s", pid1);
 char file[]="/proc/";
 strcat(file,pid1);
 strcat(file,"/status");
 FILE *input=fopen(file,"r");
 int i=1;
 child=fork();
  if(child==0)
{
   close(sockets1[1]);
   close(sockets2[1]);
   close(sockets3[1]);
   close(sockets4[1]);
   close(sockets5[1]);
  while(fscanf(input,"%s", info)!=EOF){
  if(strcmp(info,"Name: ")==0){
  fscanf(input, "%s", info1);
  write(sockets1[0], info1,100);
  close(sockets1[0]);
}
  if(strcmp(info,"State: ")==0){
  fscanf(input, "%s", info2);
  write(sockets2[0], info2,100);
  close(sockets2[0]);
}
 if(strcmp(info,"Uid: ")==0){
  fscanf(input, "%s", info3);
  write(sockets3[0], info3,100);
  close(sockets3[0]);
}
 if(strcmp(info,"PPid: ")==0){
  fscanf(input, "%s", info4);
  write(sockets4[0], info1,100);
  close(sockets4[0]);
}
 if(strcmp(info,"FDSize: ")==0){
  fscanf(input, "%s", info5);
  write(sockets5[0], info5,100);
  close(sockets5[0]);
}
}
}
 else{ //parent
   close(sockets1[0]);
   close(sockets2[0]);
   close(sockets3[0]);
   close(sockets4[0]);
   close(sockets5[0]);
   read(sockets1[1], info1,100);
   int l1=strlen(info1)+5;
   printf("%d: Name:%s\n", l1, info1);
   read(sockets2[1], info2,100);
   int l2=strlen(info2)+6;
   printf("%d: State:%s\n", l2, info2);
   read(sockets3[1], info3,100);
   int l3=strlen(info3)+3;
   printf("%d: Uid:%s\n", l3, info3);
   read(sockets4[1], info4,100);
   int l4=strlen(info4)+4;
   printf("%d: PPid:%s\n", l4, info4);
   read(sockets5[1], info5,100);
   int l5=strlen(info5)+5;
   printf("%d: FDSize:%s\n", l5, info5);

   close(sockets1[1]);
   close(sockets2[1]);
   close(sockets3[1]);
   close(sockets4[1]);
   close(sockets5[1]);
   main();
}
}
}
else
{
 if(strcmp(comanda,"logout")==0)
{
 l=0;
 int lungime=strlen("Logged out");
 printf("%d: Logged out\n", lungime);
  main();
}
 else
{

  if(strcmp(comanda,"quit")==0)
{
    exit(1);
}
  else
{
  int l=strlen("comanda gresita");
 printf("%d: comanda gresita\n",l);
 main();
}
}
}
}
}
}
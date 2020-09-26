#include <bits/stdc++.h>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>
using namespace std;
int main(int argc,char *argv[])
{   
  struct sockaddr_in server;
  bzero((char*)&server, sizeof(server));
  struct stat obj;
  char *serverIp = argv[1]; int port = atoi(argv[2]);
  struct hostent* host = gethostbyname(serverIp);
  server.sin_family = AF_INET; 
    server.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    server.sin_port = htons(port);
  int sock;
  int choice;
  char buf[100], command[5], filename[20], *f;
  int k, size, status;
  int filehandle;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    {
      printf("socket creation failed");
      exit(1);
    }
  k = connect(sock,(struct sockaddr*)&server, sizeof(server));
  if(k == -1)
    {
      printf("Connect Error");
      exit(1);
    }
  int i = 1;
  while(1)
    {
      printf("Enter a choice:\n1 - Download\n2 - Upload\n3 - quit\n");
      scanf("%d", &choice);
      switch(choice)
	{
	case 1:
	  printf("Enter filename to Download: ");
	  scanf("%s", filename);
	  strcpy(buf, "get ");
	  strcat(buf, filename);
	  send(sock, buf, 100, 0);
	  recv(sock, &size, sizeof(int), 0);
	  if(!size)
	    {
	      printf("No such file on the remote directory\n\n");
	    break;
	    }
	  f = (char *)malloc(size);
	  recv(sock, f, size, 0);
	  while(1)
	    {
	      filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
	      if(filehandle == -1)
		{
		  sprintf(filename + strlen(filename), "%d", i);
		}
	      else break;
	    }
	  write(filehandle, (void *)f, size);
	  close(filehandle);
	  strcpy(buf, "cat ");
	  strcat(buf, filename);
	  //system(buf);
	  break;
	case 2:
	  printf("Enter filename to Upload to server: ");
          scanf("%s", filename);
	  filehandle = open(filename, O_RDONLY);
          if(filehandle == -1)
            {
              printf("No such file on the local directory\n\n");
              break;
            }
          strcpy(buf, "put ");
	  strcat(buf, filename);
	  send(sock, buf, 100, 0);
	  stat(filename, &obj);
	  size = obj.st_size;
	  send(sock, &size, sizeof(int), 0);
	  sendfile(sock, filehandle, NULL, size);
	  recv(sock, &status, sizeof(int), 0);
	  if(status)
	    printf("File stored successfully\n");
	  else
	    printf("File failed to be stored to remote machine\n");
	  break;
	case 3:
	  strcpy(buf, "quit");
          send(sock, buf, 100, 0);
          recv(sock, &status, 100, 0);
	  if(status)
	    {
	      printf("Server closed\nQuitting..\n");
	      exit(0);
	    }
	    printf("Server failed to close connection\n");
	}
    }
}

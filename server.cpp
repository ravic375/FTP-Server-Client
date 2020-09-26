#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>
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
using namespace std;
int main(int argc,char *argv[])
{
  struct stat obj;
  if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    int port = atoi(argv[1]);
    sockaddr_in servAddr, client;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
    int serverSd = socket(AF_INET, SOCK_STREAM, 0), sock2;
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
  if(listen(serverSd,1) == -1)
    {
      printf("Listen failed");
      exit(1);
    }
  int len = sizeof(client);
  sock2 = accept(serverSd,(struct sockaddr*)&client, (socklen_t *)&len);
  int i = 1, filehandle, size;
  char buf[100], command[5], filename[20];
  while(1)
    {
      recv(sock2, buf, 100, 0);
      sscanf(buf, "%s", command);
      if(!strcmp(command,"get"))
	{
	  sscanf(buf, "%s%s", filename, filename);
	  stat(filename, &obj);
	  filehandle = open(filename, O_RDONLY);
        {
	  int c = 0, len;
	  char *f;
	  sscanf(buf+strlen(command), "%s", filename);
	  recv(sock2, &size, sizeof(int), 0);
	  i = 1;
	  while(1)
	    {
	      filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
	      if(filehandle == -1)
		{
		  sprintf(filename + strlen(filename), "%d", i);
		}
	      else
		break;
	    }
	  f = (char *)malloc(size);
	  recv(sock2, f, size, 0);
	  c = write(filehandle, f, size);
	  close(filehandle);
	  send(sock2, &c, sizeof(int), 0);
        }
      else if(!strcmp(command, "quit"))
	{
	  printf("FTP server quitting..\n");
	  i = 1;
	  send(sock2, &i, sizeof(int), 0);
	  exit(0);
	}
    }
  return 0;
}
	  size = obj.st_size;
	  if(filehandle == -1)
	      size = 0;
	  send(sock2, &size, sizeof(int), 0);
	  if(size)
	  sendfile(sock2, filehandle, NULL, size);
      
	}
      else if(!strcmp(command, "put"))

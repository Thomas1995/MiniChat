#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char PORT[] = "3490";
const int MAXDATASIZE = 1000;

int connectToServer() {
  struct addrinfo hints, *rez;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo(NULL, PORT, &hints, &rez);

  int sockfd = -1;

  addrinfo *p;

  for(p = rez; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    if(sockfd == -1) {
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      continue;
    }

    break;
  }

  if(p == NULL) {
    perror("Failed to connect ");
    exit(1);
  }

  freeaddrinfo(rez);
  return sockfd;
}

int main(int argc, char *argv[]) {
  if(argc != 2) {
    perror("Username not chosen");
    exit(1);
  }

  int server = connectToServer();

  printf("Connected to server\n");

  char msg[MAXDATASIZE];

  send(server, argv[1], sizeof(argv[1]), 0);

  while(true) {
    fgets(msg, sizeof msg, stdin);
    send(server, msg, strlen(msg), 0);
  }

  close(server);

  return 0;
}

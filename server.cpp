#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> // getaddrinfo
#include <unistd.h> // close
#include <string.h> // memset
#include <stdlib.h> // exit
#include <stdio.h>

const char PORT[] = "3490";
const int BACKLOG = 20;

int setupServer() {
  struct addrinfo hints, *rez;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, PORT, &hints, &rez);

  int yes = 1;
  int server = -1;

  for(addrinfo *p = rez; p != NULL; p = p->ai_next) {
    server = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    if(server == -1) {
      perror("Socket() failed: ");
      continue;
    }

    if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("Setsockopt() failed: ");
      exit(1);
    }

    if(bind(server, p->ai_addr, p->ai_addrlen) == -1) {
      perror("Bind() failed: ");
      exit(1);
    }

    if(listen(server, BACKLOG) == -1) {
      perror("Listen() failed: ");
      exit(1);
    }

    break;
  }

  freeaddrinfo(rez);
  return server;
}

int main() {
  const int server = setupServer();

  if(server == -1) {
    printf("Could not set up server\n");
    exit(1);
  }

  printf("Listening for connections\n");

  while(true) {
    sockaddr_storage their_addr;
    socklen_t sin_size = sizeof their_addr;

    int new_fd = accept(server, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }
  }

  return 0;
}

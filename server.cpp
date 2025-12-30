#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "messages/auth.hpp"
#include "messages/app.hpp"

#define SERVER_PORT  5432
#define MAX_PENDING  1
#define MAX_LINE     256

#define IP_LOCALHOST "127.0.0.1"
#define PROT_IP 0

int main() {
  struct sockaddr_in sin;
  char buf[MAX_LINE];
  int buf_len;
  socklen_t addr_len;
  int sock_fd, accept_fd;
  int enable = 1;

  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(IP_LOCALHOST);
  sin.sin_port = htons(SERVER_PORT);

  if ((sock_fd = socket(PF_INET, SOCK_STREAM, PROT_IP)) < 0) {
    perror("main: socket error");
    exit(1);
  }
  setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
  setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));

  if ((bind(sock_fd, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
    perror("main: bind error");
    exit(1);
  }
  listen(sock_fd, MAX_PENDING);

 /* wait for connection, then receive and print text */
  while(true) {
    if ((accept_fd = accept(sock_fd, (struct sockaddr *)&sin, &addr_len)) < 0) {
      perror("main: accept error");
      exit(1);
    }

    ShellRequest shellRequest = ShellRequest::unmarshal(receivegeneric(accept_fd));
    puts("Received:\n");
    puts(shellRequest.command.get());

    sendgeneric(accept_fd, shellRequest.exec().marshal());
    close(accept_fd);
  }
}
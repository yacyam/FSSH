#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PORT  5432
#define MAX_PENDING  1
#define MAX_LINE     256

#define IP_LOCALHOST "127.0.0.1"
#define PROT_IP 0

#define NUM_CMDS_SUPPORTED 2

int is_supported_cmd(char *cmd) {
  char *cmds_supported[] = {"ls\n", "pwd\n"};

  for (int i = 0; i < NUM_CMDS_SUPPORTED; i++) {
    if (strcmp(cmds_supported[i], cmd) == 0) {
      return 0;
    }
  }
  return -1;
}

int main() {
  struct sockaddr_in sin;
  char buf[MAX_LINE];
  int buf_len;
  socklen_t addr_len;
  int sock_fd, accept_fd;

  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(IP_LOCALHOST);
  sin.sin_port = htons(SERVER_PORT);

  if ((sock_fd = socket(PF_INET, SOCK_STREAM, PROT_IP)) < 0) {
    perror("main: socket error");
    exit(1);
  }
  if ((bind(sock_fd, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
    perror("main: bind error");
    exit(1);
  }
  listen(sock_fd, MAX_PENDING);

 /* wait for connection, then receive and print text */
  while(1) {
    if ((accept_fd = accept(sock_fd, (struct sockaddr *)&sin, &addr_len)) < 0) {
      perror("main: accept error");
      exit(1);
    }
    while ((buf_len = recv(accept_fd, buf, sizeof(buf), 0))) {
      fputs(buf, stdout);
      if (is_supported_cmd(buf) == 0) {
        send(accept_fd, "GOOD CMD\n", 11, 0);
      } else {
        send(accept_fd, "BAD CMD\n", 10, 0);
      }
    }
    close(accept_fd);
  }
}
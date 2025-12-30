#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "auth.hpp"
#include "app.hpp"

#define SERVER_PORT 5432
#define MAX_LINE 256

#define IP_SERVER "127.0.0.1"
#define PROT_IP 0


int main(int argc, char * argv[]) {
    FILE *fp;
    //struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    int sock_fd;
    int len;

    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    inet_aton(IP_SERVER, &sin.sin_addr);

    //bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(SERVER_PORT);

    /* active open */
    if ((sock_fd = socket(PF_INET, SOCK_STREAM, PROT_IP)) < 0) {
        perror("main: socket error");
        exit(1);
    }
    if (connect(sock_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("main: connect error");
        exit(1);
    }

    std::unique_ptr<char[]> buf = std::make_unique<char[]>(MAX_LINE);
    fgets(buf.get(), MAX_LINE, stdin);
    size_t len_cmd{strlen(buf.get())+1};

    ShellRequest shellRequest(std::move(buf), len_cmd);
    sendgeneric(sock_fd, shellRequest.marshal());

    ShellReply shellReply = ShellReply::unmarshal(receivegeneric(sock_fd));
    puts("Result:\n");
    puts(shellReply.result.get());
}
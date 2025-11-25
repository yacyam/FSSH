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

#define SERVER_PORT 5432
#define MAX_LINE 256

#define IP_LOCALHOST "127.0.0.1"
#define PROT_IP 0


int main(int argc, char * argv[]) {
    FILE *fp;
    //struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    char buf[MAX_LINE];
    int sock_fd;
    int len;

    // if (argc==2) {
    //     host = argv[1];
    // }
    // else {
    //     fprintf(stderr, "usage: simplex-talk host\n");
    //     exit(1);
    // }

    // /* translate host name into peer's IP address */
    // hp = gethostbyname(host);
    // if (!hp) {
    //     fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
    //     exit(1);
    // }

    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    inet_aton(IP_LOCALHOST, &sin.sin_addr.s_addr);

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
    /* main loop: get and send lines of text */
    while (fgets(buf, sizeof(buf), stdin)) {
        buf[MAX_LINE-1] = '\0';
        len = strlen(buf) + 1;
        send(sock_fd, buf, len, 0);
    }
}
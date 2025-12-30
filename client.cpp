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
#include "messages/auth.hpp"
// #include "messages/app.hpp"

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

    size_t uid = 4;

    SessionRequest req{uid};
    sendgeneric(sock_fd, req.marshal());

    // std::unique_ptr<char[]> buf = std::make_unique<char[]>(MAX_LINE);
    // fgets(buf.get(), MAX_LINE, stdin);
    // ShellRequest shellRequest(std::move(buf));
    // sendgeneric(sock_fd, shellRequest.marshal());

    // std::cout << "sent" << std::endl;
    // ShellReply shellReply = ShellReply::unmarshal(receivegeneric(sock_fd));
    // puts("Result:\n");
    // puts(shellReply.result.get());


    // /* main loop: get and send lines of text */
    // while (fgets(buf, sizeof(buf), stdin)) {
    //     buf[MAX_LINE-1] = '\0';
    //     len = strlen(buf) + 1;
    //     send(sock_fd, buf, len, 0);

    //     // reply from server
    //     // recv(sock_fd, buf_recv, MAX_LINE, 0);
    //     fputs(buf_recv, stdout);
    // }
}
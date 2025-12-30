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
#include "sym.hpp"

#define SERVER_PORT 5432
#define MAX_LINE 256

#define IP_SERVER "127.0.0.1"
#define PROT_IP 0

int main(int argc, char * argv[]) {
    struct sockaddr_in sin;
    int fd_sock;
    SKC skc{(char)0xBF};

    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    inet_aton(IP_SERVER, &sin.sin_addr);
    sin.sin_port = htons(SERVER_PORT);

    if ((fd_sock = socket(PF_INET, SOCK_STREAM, PROT_IP)) < 0) {
        perror("main: socket error");
        exit(1);
    }
    if (connect(fd_sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("main: connect error");
        exit(1);
    }

    std::unique_ptr<char[]> buf = std::make_unique<char[]>(MAX_LINE);
    fgets(buf.get(), MAX_LINE, stdin);
    size_t len_cmd{strlen(buf.get())+1};

    ShellRequest shellRequest(std::move(buf), len_cmd);
    sendgeneric(fd_sock, skc.encrypt(shellRequest.marshal()));

    ShellReply shellReply = ShellReply::unmarshal(skc.decrypt(receivegeneric(fd_sock)));
    puts("Result:\n");
    puts(shellReply.result.get());
}

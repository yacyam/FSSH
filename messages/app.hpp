#include <iostream>
#include <cassert>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg.hpp"

#define PIPE_READ 0
#define PIPE_WRITE 1
#define BUF_SIZ 65536

class ShellReply : public Message<ShellReply> {
public:
  std::unique_ptr<char[]> result;
  size_t len;

  ShellReply(std::unique_ptr<char[]> result, size_t len) : 
    result(std::move(result)), len(len) {}

  Bytes marshal() {
    std::unique_ptr<char[]> msg = std::make_unique<char[]>(len);
    memcpy(msg.get(), result.get(), len);
    return Bytes(std::move(msg), len);
  }

  static ShellReply unmarshal(Bytes bytes) {
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(bytes.len);
    memcpy(buf.get(), bytes.data.get(), bytes.len);
    return ShellReply(std::move(buf), bytes.len);
  }
};

class ShellRequest : public Message<ShellRequest> {
public:
  std::unique_ptr<char[]> command;
  size_t len;

  ShellRequest(std::unique_ptr<char[]> command, size_t len) : 
    command(std::move(command)), len(len) {}

  ShellReply exec() {
    std::unique_ptr<char[]> buf_result = std::make_unique<char[]>(BUF_SIZ);
    size_t total_bytes_read{0};
    memset(buf_result.get(), 0, BUF_SIZ);

    int pipefd[2];
    if (pipe(pipefd) == -1) {
      perror("main: pipe error");
      exit(1);
    }

    pid_t pid = fork();
    int status;

    if (pid == 0) {
      // child

      // convert STDOUT to the write end of the pipe
      dup2(pipefd[PIPE_WRITE], STDOUT_FILENO);
      close(pipefd[PIPE_WRITE]);
      execlp("bash", "bash", "-c", command.get(), NULL); // TODO: very dangerous
      assert(false);
    } else {
      // parent
      close(pipefd[PIPE_WRITE]);
      waitpid(pid, &status, 0);
      assert(WIFEXITED(status));

      // read all the bytes outputted by the child process
      size_t bytes_read_iter{0};
      do {
        bytes_read_iter = read(
          pipefd[PIPE_READ], &buf_result.get()[total_bytes_read], BUF_SIZ - total_bytes_read
        );
        total_bytes_read += bytes_read_iter;
      } while (bytes_read_iter > 0);
      close(pipefd[PIPE_READ]);
    }

    return ShellReply(std::move(buf_result), total_bytes_read);
  }

  Bytes marshal() {
    std::unique_ptr<char[]> msg = std::make_unique<char[]>(len);
    memcpy(msg.get(), command.get(), len);
    return Bytes(std::move(msg), len);
  }

  static ShellRequest unmarshal(Bytes bytes) {
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(bytes.len);
    memcpy(buf.get(), bytes.data.get(), bytes.len);
    return ShellRequest(std::move(buf), bytes.len);
  }
};

#ifndef MSG_HPP
#define MSG_HPP
#include <unistd.h>
#include <iostream>

// Bytes object for sending messages:
class Bytes {
public:
  std::unique_ptr<char[]> data;
  size_t len;

  Bytes(std::unique_ptr<char[]> data, size_t len) : data(std::move(data)), len(len) {}
};

inline void _write_until_finished(int fd, char *data, size_t size) {
  size_t bytes_written{0};
  while (bytes_written < size) { 
    bytes_written += write(fd, &data[bytes_written], size - bytes_written);
  }
  assert(bytes_written == size);
}

inline void _read_until_finished(int fd, char *buf, size_t size) {
  size_t bytes_read{0};
  while (bytes_read < size) {
    bytes_read += read(fd, &buf[bytes_read], size - bytes_read);
  }
  assert(bytes_read == size);
}

inline void sendgeneric(int fd_sock, Bytes bytes) {
  _write_until_finished(fd_sock, (char*)&bytes.len, sizeof(bytes.len));
  _write_until_finished(fd_sock, bytes.data.get(), bytes.len);
}

inline Bytes receivegeneric(int fd_sock) {
  size_t total_bytes_data{0};
  _read_until_finished(fd_sock, (char*)&total_bytes_data, sizeof(size_t));

  std::unique_ptr<char[]> buf_data = std::make_unique<char[]>(total_bytes_data);
  _read_until_finished(fd_sock, buf_data.get(), total_bytes_data);

  return Bytes(std::move(buf_data), total_bytes_data);
}

// TODO: maybe variadic function for generalized serialization?

template <typename Derived>
class Message {
public:
  // converts an object into a byte stream for sending through a socket
  virtual Bytes marshal() = 0;

  // converts a byte stream [len | data] into an instance of *this object
  static Derived unmarshal(Bytes bytes) { 
    return std::move(Derived::unmarshal(bytes)); 
  }
};

#endif // MSG_HPP

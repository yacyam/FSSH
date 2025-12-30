#include <stdlib.h>

// Bytes object for sending messages:
//   The "data_len_prepended" ([len | value] byte array)
//   The "len_of_concat" (the length of the entire [len | value] byte array)
class Bytes {
public:
  char *data_len_prepended;
  size_t len_of_concat;

  Bytes(char *data_len_prepended, size_t len_of_concat) : 
    data_len_prepended(data_len_prepended), len_of_concat(len_of_concat) {}
};

void sendgeneric(int fd_sock, Bytes bytes) {
  size_t bytes_written{0};
  while (bytes_written < bytes.len_of_concat) {
    bytes_written += write(fd_sock, 
                           &bytes.data_len_prepended[bytes_written], 
                            bytes.len_of_concat - bytes_written);
  }
  assert(bytes_written == bytes.len_of_concat);
}

std::unique_ptr<char[]> receivegeneric(int fd_sock) {
  // holds the length of the data in the suffix of the message
  std::unique_ptr<char[]> buf_len_prepended = std::make_unique<char[]>(sizeof(size_t));

  // read the prefix of the message 
  // (first size_t bytes determine the size of the remainder of the message)
  size_t bytes_read_in_prefix{0};
  while (bytes_read_in_prefix < sizeof(size_t)) {
    bytes_read_in_prefix += read(fd_sock, 
                                 &buf_len_prepended[bytes_read_in_prefix], 
                                 sizeof(size_t) - bytes_read_in_prefix);
  }
  assert(bytes_read_in_prefix == sizeof(size_t));

  // parse the remaining size using the prefix just read
  size_t total_bytes_data{*((size_t*)buf_len_prepended.get())};
  size_t total_bytes_full_msg{sizeof(size_t) + total_bytes_data};

  std::unique_ptr<char[]> buf_full_msg = std::make_unique<char[]>(total_bytes_full_msg);
  memcpy(buf_full_msg.get(), buf_len_prepended.get(), sizeof(size_t));

  // read the remainder of the message
  size_t bytes_read_full_msg{sizeof(size_t)};
  while (bytes_read_full_msg < total_bytes_full_msg) {
    bytes_read_full_msg += read(fd_sock, 
                                &buf_full_msg[bytes_read_full_msg], 
                                total_bytes_full_msg - bytes_read_full_msg);
  }
  assert(bytes_read_full_msg == total_bytes_full_msg);
  return std::move(buf_full_msg);
}

// TODO: maybe variadic function for generalized serialization?

template <typename Derived>
class Message {
public:
  // converts an object into a byte stream for sending through a socket
  virtual Bytes marshal() = 0;

  // converts a byte stream [len | data] into an instance of *this object
  static Derived unmarshal(std::unique_ptr<char[]> data_len_prepended) { 
    return std::move(Derived::unmarshal(data_len_prepended)); 
  }
};

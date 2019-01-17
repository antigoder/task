#include "util.h"

namespace util {

std::string get_format_time(std::string &format) {
  return get_format_time(format.c_str());
}

std::string get_format_time(const char *format) {
  time_t  timer;

  timer = time(NULL);
  struct tm *stm = localtime(&timer);

  char ts[32];
  size_t len = strftime(ts, 32, format, stm);

  return std::string(ts, len);
}

std::string get_short_filename(std::string &filename) {
  return get_short_filename(filename.c_str());
}

std::string get_short_filename(const char *filename) {
  if (filename == NULL || *filename != '/') {
    return std::string(filename);
  }
  const char *pos = strrchr(filename, '/');
  return (pos) ? (pos + 1) : filename;
}

ssize_t writen(int fd, const void *buf, size_t n) {
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;
  
  ptr = (const char *)buf;
  nleft = n;
  while (nleft > 0) {
    if ((nwritten = write(fd, buf, nleft)) <= 0) {
      if (nwritten < 0 && errno == EINTR) {
        nwritten = 0;
      } else {
        return -1;
      }
    }
    nleft -= nwritten;
    ptr += nwritten;
  }
  return n;
}

} // namespace util

#ifndef TM_H_
#define TM_H_

#include <string>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>

namespace util {

std::string get_format_time(std::string &format);
std::string get_format_time(const char *format);

std::string get_short_filename(std::string &filename);
std::string get_short_filename(const char *filename);

ssize_t writen(int fd, const void *buf, size_t n);

}

#endif

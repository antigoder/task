#ifndef LOG_H_
#define LOG_H_

#include <util.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef enum log_level {
  ERROR = 0,
  WARN,
  INFO,
  DEBUG
} log_level_t;

class Log {
 public:
  static Log * GetInstance() {
    static Log log;
    return &log;
  }

  int Init(log_level_t level = DEBUG, const std::string &filename = "", uint64_t line_size = 256,
           uint64_t rotate_size = 64);

  int WriteLog(log_level_t level, const char *file,
               const char *func, size_t line, const char* format, ...); 

 private:
  Log();
  ~Log();

  int rotate(const std::string &filename);
  std::string get_log_level(log_level_t level);

  int fd_;
  int max_line_size_;
  log_level_t level_;
  uint64_t log_cur_size_;
  uint64_t log_rotate_size_;

  pthread_mutex_t mutex_;

};

#define LOG_ERROR(msg, ...) Log::GetInstance()->WriteLog(ERROR, __BASE_FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) Log::GetInstance()->WriteLog(WARN, __BASE_FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) Log::GetInstance()->WriteLog(INFO, __BASE_FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) Log::GetInstance()->WriteLog(DEBUG, __BASE_FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)

#endif

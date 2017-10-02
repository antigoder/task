#include "log.h"

Log::Log() {
  mutex_ = PTHREAD_MUTEX_INITIALIZER;

  Init();
}

Log::~Log() {
  pthread_mutex_destroy(&mutex_);
}

int Log::Init(log_level_t level, const std::string &filename, uint64_t line_size,
              uint64_t rotate_size) {
  max_line_size_ = line_size;
  level_ = level;
  log_rotate_size_ = rotate_size * 1024ul * 1024ul;

  fd_ = fileno(stdout); // default log output

  if (!filename.empty()){
    fd_ = open(filename.c_str(), O_CREAT | O_RDWR | O_APPEND, 0664);
  }
  if (fd_ < 0) {
    fprintf(stderr, "Open log file failed.");
    return -1;
  }

  struct stat filestat;
  if (fstat(fd_, &filestat) < 0) {
    fprintf(stderr, "Get file stat failed");
    return -2;
  }
  log_cur_size_ = filestat.st_size;

  return 0;
}

int Log::WriteLog(log_level_t level, const char *file,
                  const char *func, size_t line, const char *format, ...) {
  char buf[max_line_size_];
  int len = 0;
  if (level > level_) {
    return 0;
  }
  std::string log_level = get_log_level(level);

  std::string log_time = util::get_format_time("%Y%m%d %H:%M:%S");
  std::string log_filename = util::get_short_filename(file);

  pid_t log_pid = getpid();
  pid_t log_tid = syscall(SYS_gettid); // get kernel thread id; pthread_self() return posix thread id

  len = snprintf(buf, sizeof(buf), "%s %d %d [%s] %s %s():%lu ", 
                 log_time.c_str(), log_pid, log_tid, log_level.c_str(),
                 log_filename.c_str(), func, line);
  va_list args;
  va_start(args, format);
  len += vsnprintf(buf + len, sizeof(buf) - len - 1, format, args);
  va_end(args);
  
  buf[len++] = '\n';
  
  pthread_mutex_lock(&mutex_);

  len = util::writen(fd_, buf, len);
  if (len < 0) {
    fprintf(stderr, "written log to file failed. len:%d", len);
  }

  log_cur_size_ += len;
  if (log_cur_size_ >= log_rotate_size_) {
    rotate(std::string(file, strlen(file)));
  }

  pthread_mutex_unlock(&mutex_);

  return len;
}

// private

int Log::rotate(const std::string &filename) {
  if (fd_ < 3) {  // ignore stdin stdout stderr
    fprintf(stderr, "Invaild fd:%d", fd_);
    return 0;
  }
  
  std::string tm = util::get_format_time("%Y%m%d-%H%M%S");
  std::string rotate_name = filename + "." + tm;
  if (rename(filename.c_str(), rotate_name.c_str()) < 0) {
    fprintf(stderr, "Rename filename failed");
    return -1;
  }
  close(fd_);

  fd_ = open(filename.c_str(), O_CREAT | O_RDWR | O_APPEND, 0664);
  if (fd_ < 0) {
    fprintf(stderr, "Open new log file failed");
    return -2;
  }
  
  return 0;
}

std::string Log::get_log_level(log_level_t level) {
  std::string log_level = "DEBUG";
  switch (level) {
    case ERROR:
      log_level = "ERR";
      break;
    case WARN:
      log_level = "WRN";
      break;
    case INFO:
      log_level = "INF";
      break;
    case DEBUG:
    default:
      log_level = "DEBUG";
      break;
  }
  return log_level;
}


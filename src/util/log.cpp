#include "log.h"

#define MAX_BUFSIZE 1024 * 32

char *Log::msgbuf_ = NULL;
int Log::fd_ = -1;
pthread_mutex_t Log::mutex_ = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Log::cond_ = PTHREAD_COND_INITIALIZER;

Log::Log() {
  msgbuf_ = new char[MAX_BUFSIZE];   // 32k

  Init();
}

Log::~Log() {
  pthread_mutex_destroy(&mutex_);
  pthread_cond_destroy(&cond_);
}

int Log::Init(log_level_t level, const std::string &filename, uint64_t line_size,
              uint64_t rotate_size) {
  level_ = level;
  max_line_size_ = line_size;
  log_rotate_size_ = rotate_size * 1024ul * 1024ul;
  
  if (open_log_file(filename) < 0) {
    return -1;
  }

  if (start_dump_thread() < 0) {
    return -2;
  }

  return 0;
}

int Log::WriteLog(log_level_t level, const char *file,
                  const char *func, size_t line, const char *format, ...) {
  char msg[max_line_size_];
  int len = 0;
  if (level > level_) {
    return 0;
  }
  std::string log_level = get_log_level(level);

  std::string log_time = util::get_format_time("%Y%m%d %H:%M:%S");
  std::string log_filename = util::get_short_filename(file);

  len = snprintf(msg, max_line_size_, "%s %d %d [%s] %s %s():%lu ", 
                 log_time.c_str(), getpid(), syscall(SYS_gettid),
                 log_level.c_str(), log_filename.c_str(), func, line);
  va_list args;
  va_start(args, format);
  len += vsnprintf(msg + len, sizeof(msg) - len - 1, format, args);
  va_end(args);
  
  msg[len++] = '\n';
  
  pthread_mutex_lock(&mutex_);
  
  while (strlen(msgbuf_) + len > MAX_BUFSIZE) {
    pthread_cond_wait(&cond_, &mutex_);
  }
  memcpy(msgbuf_ + strlen(msgbuf_), msg, len);

  log_cur_size_ += len;
  if (log_cur_size_ >= log_rotate_size_) {
    rotate(std::string(file, strlen(file)));
  }

  pthread_mutex_unlock(&mutex_);

  return len;
}

// private

int Log::open_log_file(const std::string &filename) {
  fd_ = fileno(stdout); // default log output
  if (!filename.empty()){
    fd_ = open(filename.c_str(), O_CREAT | O_RDWR | O_APPEND, 0664);
  }
  if (fd_ < 0) {
    fprintf(stderr, "Open log file failed\n");
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

int Log::start_dump_thread() {
  pthread_t tid;
  pthread_attr_t attr;

  if (pthread_attr_init(&attr) != 0) {
    fprintf(stderr, "init thread attr failed\n");
    return -1;
  }

  if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
    fprintf(stderr, "set thread attr failed\n");
    return -2;
  }

  if (pthread_create(&tid, &attr, dump_to_file, NULL) != 0) {
    fprintf(stderr, "create dump log file thread failed\n");
    return -3;
  }
  
  if (pthread_attr_destroy(&attr) != 0) {
    fprintf(stderr, "destroy thread attr failed\n");
    return -4;
  }

  return 0;
}

void * Log::dump_to_file(void *msg) {
  struct timeval tv;
  
  tv.tv_sec = 0;
  tv.tv_usec = 5000;

  while (true) {
    pthread_mutex_lock(&mutex_);
    int len = strlen(msgbuf_);
    while (len) {
      len -= util::writen(fd_, msgbuf_, len);
    }
    memset(msgbuf_, 0, strlen(msgbuf_));
    pthread_mutex_unlock(&mutex_);
    if (len == 0) {
      pthread_cond_signal(&cond_);
    }
    select(0, NULL, NULL, NULL, &tv);
  }

  pthread_exit(NULL);

  return NULL;
}

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
    case DEBUG:
      log_level = "DEBUG";
      break;
    case INFO:
    default:
      log_level = "INF";
      break;
  }
  return log_level;
}


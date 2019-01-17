#include "service.h"

Service::Service() { }

Service::~Service() { }

int Service::Init() {
  Config *config = Config::GetInstance();
  ip_ = config->ip;
  port_ = config->port;

  return 0;
}

int Service::Run() {
  int epfd = epoll_create1(EPOLL_CLOEXEC);
  if (epfd < 0) {
    LOG_ERROR("epoll create failed. error:%s", strerror(errno));
  }

  close(epfd);

  return 0;
}

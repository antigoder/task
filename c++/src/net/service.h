#ifndef SERVICE_H_
#define SERVICE_H_

#include <log.h>
#include <config.h>
#include <string>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <sys/epoll.h>

class Service {
 public:
  static Service * GetInstance() {
    static Service srv;
    return &srv;
  }

  int Init();
  
  int Run();

 private:
  Service();
  ~Service();
  
  std::string ip_;
  uint16_t port_;

};

#endif

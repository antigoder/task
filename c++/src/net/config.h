#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <stdint.h>

class Config {
 public:
  static Config * GetInstance() {
    static Config conf;
    return &conf;
  }

  std::string ip;
  uint16_t port;

};

#endif

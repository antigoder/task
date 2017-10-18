#ifndef WORK_H_
#define WORK_H_

#include <iostream>
#include <task.h>
#include <log.h>

class Work {
 public:
  int Start();
  virtual void Run() = 0;

 private:
  static void* worker(void *arg);
};

#endif

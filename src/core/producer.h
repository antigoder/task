#ifndef PRODUCER_H_
#define PRODUCER_H_

#include "work.h"
#include <log.h>

class Producer : public Work {
 public:
  Producer(task_pool_t *task_pool);
  void Run();
 private:
  static int seq;
  task_pool_t *task_pool_;
};

#endif


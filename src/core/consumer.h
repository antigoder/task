#ifndef CONSUMER_H_
#define CONSUMER_H_

#include "work.h"
#include <log.h>

class Consumer : public Work {
 public:
  Consumer(task_pool_t *task_pool);
  void Run();
 private:
  task_pool_t *task_pool_;
};

#endif

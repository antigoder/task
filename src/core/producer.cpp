#include "producer.h"

int Producer::seq = 0;

Producer::Producer(task_pool_t *task_pool) {
  LOG_DEBUG("Create a producer");
  task_pool_ = task_pool;
}

void Producer::Run() {
  while (true) {
    if (task_pool_->Push(new Task(seq))) {
      LOG_INFO("Produce: %d", seq++);
    }
    sleep(1);
  }
}

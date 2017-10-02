#include "consumer.h"

Consumer::Consumer(task_pool_t *task_pool) {
  LOG_DEBUG("Create a consumer");
  task_pool_ = task_pool;
}

void Consumer::Run() {
  while (true) {
    sleep(1);
    task_t *task = task_pool_->Pop();
    if (task != NULL) {
      LOG_INFO("Consume: %d", task->seq);
      delete task;
    }
  }
}

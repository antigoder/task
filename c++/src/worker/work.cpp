#include "work.h"

int Work::Start() {
  int ret = 0;
  pthread_t tid;
  pthread_attr_t attr;

  pthread_attr_init(&attr);

  if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
    LOG_WARN("pthread set detach failed");
  }

  ret = pthread_create(&tid, &attr, worker, this);
  if (ret != 0) {
    LOG_ERROR("pthread create failed.");
    return -1;
  }
  return 0;
}

void* Work::worker(void *arg) {
  Work *ptr = (Work *)arg;
  ptr->Run();
  return NULL;
}


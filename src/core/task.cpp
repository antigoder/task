#include "task.h"

TaskPool::TaskPool() {
  head_ = NULL;
  counter_ = 0;

  mutex_ = PTHREAD_MUTEX_INITIALIZER;
  cond_ = PTHREAD_COND_INITIALIZER;
}

TaskPool::~TaskPool() {
  task_t *p = NULL;
  p = head_;
  while (p != NULL) {
    head_ = head_->next;
    delete p;
    p = head_;
  }
  counter_ = 0;

  pthread_mutex_destroy(&mutex_);
  pthread_cond_destroy(&cond_);
}

bool TaskPool::Push(task_t *task) {
  pthread_mutex_lock(&mutex_);
  if (task == NULL) {
    LOG_WARN("Can not push a NULL task");
    return false;
  }

  while (counter_ > 10) {
    LOG_DEBUG("task queue is full, waitting for consume...");
    pthread_cond_wait(&cond_, &mutex_);
  }

  if (head_ == NULL) {
    head_ = task;
    head_->next = NULL;
    counter_++;
    pthread_mutex_unlock(&mutex_);
    pthread_cond_signal(&cond_);
    return true;
  }

  task_t *p = head_;
  while (p->next != NULL) {
    p = p->next;
  }
  
  p->next = task;
  counter_++;
  pthread_mutex_unlock(&mutex_);
  pthread_cond_signal(&cond_);
  return true;
}

task_t * TaskPool::Pop() {
  pthread_mutex_lock(&mutex_);
  while (head_ == NULL) {
    LOG_DEBUG("task queue is empty, waitting...");
    pthread_cond_wait(&cond_, &mutex_);
  } 

  task_t *p = head_;
  head_ = head_->next;
  counter_--;
  pthread_mutex_unlock(&mutex_);
  pthread_cond_signal(&cond_);
  return p;
}

#ifndef TASK_H_
#define TASK_H_

#include <pthread.h>
#include <string>
#include <iostream>
#include <log.h>

using namespace std;

typedef struct Data {
  std::string key;
  std::string val;
} data_t;

typedef struct Task {
  int seq;
//  void *data;
  Task* next;

  Task() {
    seq = 0;
    next = NULL;
  }
  Task(int s) {
    seq = s;
    next = NULL;
  }

} task_t;

typedef class TaskPool {
 public:
  TaskPool();
  ~TaskPool();

  bool Push(task_t *task);
  Task* Pop();
  
  void ListTask() {
    task_t *p = head_;
    while (p != NULL) {
      std::cout << p->seq << std::endl;
      p = p->next;
    }
  }

 private:
  task_t *head_;
  int counter_;
  
  pthread_mutex_t mutex_;
  pthread_cond_t  cond_;

} task_pool_t;

#endif

#include <log.h>
#include <task.h>
#include <producer.h>
#include <consumer.h>

using namespace std;

int main(int argc, char* argv[]) {
  
  Log::GetInstance()->Init(DEBUG);

  task_pool_t task_pool;

  Producer *producer1 = new Producer(&task_pool);
  Producer *producer2 = new Producer(&task_pool);
  Consumer *consumer1 = new Consumer(&task_pool);

  producer1->Start();
  producer2->Start();
  consumer1->Start();
  
  
  while (true) {
    sleep(10);
  }
  
  return 0;
}

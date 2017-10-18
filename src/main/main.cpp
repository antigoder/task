#include <log.h>
#include <task.h>
#include <producer.h>
#include <consumer.h>
#include <storage.h>
#include <service.h>

using namespace std;

int main(int argc, char* argv[]) {

  Log::GetInstance()->Init(DEBUG);

  Service *srv = Service::GetInstance();

  srv->Init();

  srv->Run();

  /*
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
  */
  /*
  Storage *st = Storage::GetInstance();
  st->DBTest();
  */
  usleep(10000);

  return 0;
}

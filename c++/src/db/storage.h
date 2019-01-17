#ifndef STORAGE_H_
#define STORAGE_H_

#include <log.h>
#include <leveldb/db.h>
#include <string>

class Storage {
 public:
  static Storage * GetInstance() {
    static Storage st;
    return &st;
  }
  
  int Init(const std::string &dbpath);

  void DBTest();

 private:
  Storage();
  ~Storage();
  
  std::string dbpath_;
  leveldb::DB *db_;
  leveldb::Options options_;
  leveldb::Status status_;

};

#endif

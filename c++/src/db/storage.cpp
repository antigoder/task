#include "storage.h"

Storage::Storage() { }

Storage::~Storage() { }

int Storage::Init(const std::string &dbpath) {
  dbpath_ = dbpath;
  db_ = NULL;
  options_.create_if_missing = true;
 
  status_ = leveldb::DB::Open(options_, dbpath_, &db_);
  LOG_DEBUG(status_.ToString().c_str());
  if (!status_.ok()) {
    LOG_ERROR("Open db file failed:%s", dbpath_.c_str());
    return -1;
  }
  return 0;
}

void Storage::DBTest() {
  if (Init("testdb") < 0) {
    return;
  } 

  std::string key1="people";
  std::string value1="jason";
  std::string value;
  status_ = db_->Put(leveldb::WriteOptions(),key1,value1);
  if(status_.ok())
    status_ = db_->Get(leveldb::ReadOptions(),"people",&value);
  if(status_.ok())
    LOG_INFO(value.c_str());
  else
    LOG_INFO(status_.ToString().c_str());
}


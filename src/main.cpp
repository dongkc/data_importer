#include <Poco/File.h>
#include <Poco/SharedMemory.h>
#include <Poco/DataTimeParser.h>

#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <vector>
#include <iostream>
#include <glog/logging.h>

using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

using namespace Poco;

std::string to_str(const unsigned char byte)
{
  char buf[2];
  char tmp  = ((byte & 0xF0) >> 4);
  if (tmp > 9) {
    buf[0] = tmp - 10 + 'A';
  } else {
    buf[0] = tmp + '0';
  }
  tmp = byte & 0x0F;
  if (tmp > 9) {
    buf[1] = tmp - 10 + 'A';
  } else {
    buf[1] = tmp + '0';
  }
  std::string res(buf, 2);

  return res;
}

struct LockFrame
{
  std::string    lock_id;
  std::string    message_id;
  Poco::DateTime datetime;
  std::string    frame;
};

//解析一个frame，移动指针到下一个frame的开始，如果出错，设置标志位error
char* parse(char* begin, char* end, LockFrame& lock_frame, bool& error)
{
  char* offset = begin;

  if (*offset != 0xFF) {
    LOG(ERROR) << "Parse error, not begin with magic id 0xFF";
    error = true;
    return begin;
  }

  string timestamp(++offset, 19);
  DateTime datetime;
  int timezone;
  if (!DateTimeParser::tryParse(DateTimeFormat::SORTABLE_FORMAT, timestamp, lock_frame.datetime, timezone)) {
    LOG(ERROR) << "Parse timestamp error";
    error = true;
    return begin;
  }

  offset += 19;
  uint16_t len;
  memcpy(&len, offset, 2);

  if (*offset != 0x7E || *(offset + len) != 0x7E) {
    LOG(ERROR) << "Invalid frame, not begin or end with 0x7E";
    error = true;
    return begin;
  }

  offset++;
  lock_frame.message_id_ = to_str(*offset) + to_str(*(offset + 1));

  string phone_num;
  for (int i = 4; i < 10; ++i) {
    phone_num += to_str(*(offset + i));
  }

  lock_frame.lock_id = phone_num;
  lock_frame.frame = string(--offset, len);

  return offset + len;
}

int main(int argc, char** argv)
{
  Poco::Data::MySQL::Connector::registerConnector();
  Session session("MySQL", "host=localhost;port=3306;db=test;user=root;password=dongkai;compress=true;auto-reconnect=true");

  LOG(INFO) << "Database connect successfull";
  //session << "DROP TABLE IF EXISTS gps_data", now;

  session << "CREATE TABLE IF NOT EXISTS gps_data(lock_id VARCHAR(12), timestamp TIMESTAMP, message_id VARCHAR(4), frame blob)", now;
  LOG(INFO) << "CREATE TABLE DONE";

  File f("raw_data.dat");
  SharedMemory mem(f, SharedMemory::AM_READ);

  for (char* ptr = mem.begin(); ptr != mem.(); ++ptr) {

  }

#if 0
  // insert some rows
  Person person =
  {
    "Bart Simpson",
    "Springfield",
    12
  };

  Statement insert(session);
  insert << "INSERT INTO Person VALUES(?, ?, ?);",
         use(person.name),
         use(person.address),
         use(person.age);

  insert.execute();

  person.name    = "Lisa Simpson";
  person.address = "Springfield";
  person.age     = 10;

  insert.execute();

  person.name    = "Lisa Simpson";
  person.address = "Springfield";
  person.age     = 14;

  insert.execute();

  // a simple query
  Statement select(session);
  select << "SELECT Name, Address, Age FROM Person;",
         into(person.name),
         into(person.address),
         into(person.age),
         range(0, 1); //  iterate over result set one row at a time

  while (!select.done())
  {
    select.execute();
    std::cout << person.name << " " << person.address << " " << person.age << std::endl;
  }

#endif
  return 0;
}

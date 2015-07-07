#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <vector>
#include <iostream>
#include <glog/logging.h>

using namespace std;
using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

struct Person
{
  std::string name;
  std::string address;
  int         age;
};

int main(int argc, char** argv)
{
  Poco::Data::MySQL::Connector::registerConnector();

  // create a session
  Session session("MySQL", "host=localhost;port=3306;db=test;user=root;password=dongkai;compress=true;auto-reconnect=true");

  LOG(INFO) << "Database connect successfull";
  // drop sample table, if it exists
  session << "DROP TABLE IF EXISTS gps_data", now;

  // (re)create table
  //session << "CREATE TABLE Person (Name TIMESTAMP, Address VARCHAR(100), Age INTEGER(3))", now;
  session << "CREATE TABLE gps_data(lock_id VARCHAR(10), timestamp TIMESTAMP, message_id VARCHAR(5), frame blob)", now;
  LOG(INFO) << "CREATE TABLE DONE";

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

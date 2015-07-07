ADD_EXECUTABLE(gps_importer
  main.cpp)

TARGET_LINK_LIBRARIES(gps_importer
  pthread
  gflags
  glog
  PocoData
  PocoDataMySQL
  PocoFoundation
  boost_system
  boost_thread)

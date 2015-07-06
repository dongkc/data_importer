ADD_EXECUTABLE(gps_importer
  main.cpp)

TARGET_LINK_LIBRARIES(gps_importer
  pthread
  gflags
  glog
  boost_system
  boost_thread)

cmake_minimum_required(VERSION 3.5.1)
project(covert_channel)

set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(covert_channel main.cpp server.cpp server.h client.cpp client.h morse.cpp morse.h)
target_link_libraries(covert_channel curl)
target_link_libraries(covert_channel "${CMAKE_SOURCE_DIR}/lib/linux/libPocoNetd.so")
target_link_libraries(covert_channel "${CMAKE_SOURCE_DIR}/lib/linux/libPocoUtil.so")
target_link_libraries(covert_channel "${CMAKE_SOURCE_DIR}/lib/linux/libPocoDatad.so")
target_link_libraries(covert_channel "${CMAKE_SOURCE_DIR}/lib/linux/libPocoDataSQLite.so")
target_link_libraries(covert_channel "${CMAKE_SOURCE_DIR}/lib/linux/libPocoFoundation.so")
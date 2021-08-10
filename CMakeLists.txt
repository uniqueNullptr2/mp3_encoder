cmake_minimum_required(VERSION 3.0.0)
project(mp3_encoder VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(mp3_encoder main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(mp3_encoder PRIVATE Threads::Threads)



target_include_directories(mp3_encoder PRIVATE lib/)
Target_link_libraries(mp3_encoder PRIVATE libmp3lame.a)
Target_link_directories(mp3_encoder PRIVATE lib/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

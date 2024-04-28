cmake_minimum_required(VERSION 3.27)
project(cpp_concurrency_v2)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_concurrency_v2
#        codes/2/2.2.cpp
#        codes/2/2.3.cpp
#        codes/2/2.4.cpp
#        codes/3/3.2.3.cpp
#        codes/3/3.3.1.cpp
        codes/3/3.3.2.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(cpp_concurrency_v2 Threads::Threads)

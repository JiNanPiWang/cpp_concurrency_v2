cmake_minimum_required(VERSION 3.2)
project(cpp_concurrency_v2)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_concurrency_v2
#        codes/2/2.2.cpp
#        codes/2/2.3.cpp
#        codes/2/2.4.cpp
#        codes/3/3.2.3.cpp
#        codes/3/3.3.1.cpp
#        codes/3/3.3.2.cpp
#        codes/4/4.1.1.cpp
#        codes/4/4.1.2.cpp
#        codes/4/4.2.1.cpp
#        codes/4/4.2.2.cpp
#        codes/4/4.2.3.cpp
#        codes/4/4.2.4.cpp
#        codes/4/4.2.5.cpp
#        codes/4/4.3.2.cpp
#        codes/4/4.3.3.cpp
#        codes/4/4.4.1.cpp
#        codes/5/5.2.3.cpp
#        codes/5/5.2.4.cpp
#        codes/5/5.2.5.cpp
#        codes/5/5.2.6.cpp
#        codes/5/5.2.7.cpp
#        codes/5/5.3.3.cpp
#        codes/5/5.3.5.cpp
#        codes/6/6.2.2.cpp
#        codes/6/6.2.3.cpp
#        codes/6/6.2.3-code6.4.cpp
#        codes/6/6.2.3-code6.5.cpp
#        codes/6/6.2.3-code6.6.cpp
#        codes/6/6.2.3-code6.9.cpp
#        codes/6/6.2.3-code6.10.cpp
#        codes/6/6.3.1-code6.11.cpp
#        codes/7/7.2.1-code7.2.cpp
        codes/7/7.2.1-code7.3.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(cpp_concurrency_v2 Threads::Threads)

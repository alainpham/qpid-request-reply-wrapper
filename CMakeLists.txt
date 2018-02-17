cmake_minimum_required(VERSION 2.8)

project (qpid-request-reply-wrapper)
find_package(ProtonCpp REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

link_libraries(${ProtonCpp_LIBRARIES})
add_definitions(${ProtonCpp_DEFINITIONS})
include_directories(${ProtonCpp_INCLUDE_DIRS})

#Bring the headers, such as Student.h into the project
include_directories(include)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(qpid-request-reply-wrapper ${SOURCES})
target_link_libraries(qpid-request-reply-wrapper ${CMAKE_THREAD_LIBS_INIT} )

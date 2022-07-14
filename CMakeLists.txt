cmake_minimum_required(VERSION 3.22)
project(XPlane11_Socket C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(XPlane11_Socket_Server server_socket.c)
add_executable(XPlane11_Socket_Client client_socket.c)

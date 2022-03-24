cmake_minimum_required(VERSION 3.21)
project(dev_dynamo)

set(CMAKE_CXX_STANDARD 14)

add_executable(dev_dynamo main.cpp src/data-structures/linked-lists/Node.h src/data-structures/linked-lists/LinkedList.h src/data-structures/queue/Queue.h)

cmake_minimum_required(VERSION 3.10)

project(StdTest_Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat")

add_executable(StdTest_Test StdTest_Test.cpp)

install(
	FILES
		StdTest.h
		StdTest.inl
	DESTINATION
		include/StdTest)
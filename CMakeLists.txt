

cmake_minimum_required (VERSION 3.8)

project(Scheduler)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

include_directories("include")

file(GLOB_RECURSE SOURCE_FILE
					src/*
					include/*)

add_executable ("${PROJECT_NAME}" "${SOURCE_FILE}")
	

# GTestenable_testing()
#include_directories(${GTEST_INCLUDE_DIRS})
# find_package(GTest REQUIRED)
# Unit Tests
# Add test cpp file
#add_executable (runUnitTests "test/myTest.cpp")
# Link test executable against gtest & gtest_main
#target_link_libraries(runUnitTests ${GTEST_BOTH_LIBRARIES})
#add_test( runUnitTests runUnitTests )


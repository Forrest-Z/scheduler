
cmake_minimum_required (VERSION 3.8)

project(Scheduler)

include_directories("include")

file(GLOB_RECURSE SOURCE_FILE
					src/*
					include/*)

add_executable ("${PROJECT_NAME}" "${SOURCE_FILE}")

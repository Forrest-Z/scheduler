# CMakeList.txt: Scheduler 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(Scheduler)

include_directories(include)

file(GLOB_RECURSE SOURCE_FILE
					src/*)
# 将源代码添加到此项目的可执行文件。
add_executable ("${PROJECT_NAME}" "${SOURCE_FILE}")

# TODO: 如有需要，请添加测试并安装目标。

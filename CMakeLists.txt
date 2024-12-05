cmake_minimum_required(VERSION 3.28)
project(tiny-parser)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(include)

find_package(cJSON REQUIRED)
find_library(CJSON_LIBRARIES NAMES cjson)

add_executable(parser src/main.c src/scanner.c src/parser.c)
target_link_libraries(parser ${CJSON_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

cmake_minimum_required(VERSION 3.28)
project(tiny-parser)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(GRAPHVIZ REQUIRED libgvc libcgraph libcdt)

include_directories(${GTK4_INCLUDE_DIRS} include)
link_directories(${GTK4_LIBRARY_DIRS})

aux_source_directory(src SRC)

add_executable(parser ${SRC})
target_link_libraries(parser ${GTK4_LIBRARIES} ${GRAPHVIZ_LIBRARIES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

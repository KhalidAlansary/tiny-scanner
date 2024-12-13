cmake_minimum_required(VERSION 3.28)

project(
  tiny-parser
  VERSION 2.0
  LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(GRAPHVIZ_DIR "C:/Program Files/Graphviz")
set(GRAPHVIZ_INCLUDE_DIR "${GRAPHVIZ_DIR}/include/graphviz")
set(GRAPHVIZ_LIB_DIR "${GRAPHVIZ_DIR}/lib")

include_directories(include ${GRAPHVIZ_INCLUDE_DIRS})
link_directories(${GRAPHVIZ_LIB_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES src/main.cpp src/mainwindow.cpp include/mainwindow.hpp
                    src/mainwindow.ui src/parser.cpp src/scanner.cpp src/util.c include/parser.h)

qt_add_executable(tiny-parser ${PROJECT_SOURCES})

target_link_libraries(tiny-parser PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${GRAPHVIZ_LIBRARIES})

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.tiny-parser)
endif()
set_target_properties(
  tiny-parser
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(tiny-parser)
endif()

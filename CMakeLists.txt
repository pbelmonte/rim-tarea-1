cmake_minimum_required(VERSION 3.12)
project(rim-tarea-1)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(extraccion extraccion.cpp Video.cpp Video.h utils.h utils.cpp Frame.cpp Frame.h)

target_link_libraries(extraccion ${OpenCV_LIBS})

add_executable(busqueda busqueda.cpp Video.cpp Video.h utils.h utils.cpp Frame.cpp Frame.h)

target_link_libraries(busqueda ${OpenCV_LIBS})
cmake_minimum_required(VERSION 3.13.4)

project(ssync C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${GTK_LIBRARY_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${GTK_CFLAGS_OTHER})

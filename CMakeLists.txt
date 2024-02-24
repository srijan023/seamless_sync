cmake_minimum_required(VERSION 3.13.4)

project(ssync C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${GTK_LIBRARY_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${GTK_CFLAGS_OTHER})

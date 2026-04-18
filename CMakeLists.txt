cmake_minimum_required(VERSION 3.15)
project(projectHello C)

add_library(hello SHARED src/hello.c)

install(TARGETS hello DESTINATION my_package)
install(FILES src/hello_py.h DESTINATION my_package)
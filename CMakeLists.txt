cmake_minimum_required(VERSION 3.0)

project(treeflix)

find_package(Curses REQUIRED)

include_directories(include)

add_executable(treeflix src/main.c src/movie.c src/buffer.c)

target_link_libraries(treeflix ${CURSES_LIBRARIES})

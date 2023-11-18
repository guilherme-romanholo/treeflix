cmake_minimum_required(VERSION 3.0)

project(treeflix)

find_package(Curses REQUIRED)

include_directories(include)

add_executable(treeflix src/main/main.c src/movie/movie.c src/bptree/bp_tree.c src/bptree/node.c src/interface/interface.c src/list/list.c)

target_link_libraries(treeflix ${CURSES_LIBRARIES} m)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/CleanAll.cmake
)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
    "root.dat"
    "movies.dat"
    "ibtree.idx"
)

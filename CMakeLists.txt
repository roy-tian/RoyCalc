cmake_minimum_required (VERSION 3.0.0)
project (RoyCalc VERSION 0.1.0)

set (CMAKE_C_STANDARD 11)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out)
set (CMAKE_C_FLAGS -mwindows)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)

include_directories (${GTK3_INCLUDE_DIRS})
link_directories (${GTK3_LIBRARY_DIRS})

add_definitions (${GTK3_CFLAGS_OTHER})
add_executable (RoyCalc main.c calc.c)

target_link_libraries (RoyCalc ${GTK3_LIBRARIES})
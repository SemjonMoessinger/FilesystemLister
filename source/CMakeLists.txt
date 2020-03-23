cmake_minimum_required (VERSION 3.8)

add_executable (FilesystemLister main.cpp Helper.h Helper.cpp)
set_property(TARGET FilesystemLister PROPERTY CXX_STANDARD 17)

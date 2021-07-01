cmake_minimum_required(VERSION 3.14)

project(Abacus)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


add_subdirectory(src)
add_subdirectory(tests)

add_subdirectory(third_party/Catch2)
add_subdirectory(third_party/fmt)

cmake_minimum_required(VERSION 3.14)

project(Abacus)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(${SANITAZE} STREQUAL "FSAN")
    add_compile_options("-fsanitize=address")
    add_link_options("-fsanitize=address")
    message(${SANITAZE})
endif()

add_subdirectory(src)
add_subdirectory(tests)

add_subdirectory(third_party/Catch2)
add_subdirectory(third_party/fmt)

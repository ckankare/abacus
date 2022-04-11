set(SOURCES
    main.cpp
)

add_subdirectory(common)
add_subdirectory(abacus)

add_executable(abacus ${SOURCES})

target_compile_features(abacus PUBLIC cxx_std_20)
target_compile_options(abacus PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wconversion -Wextra -Wpedantic>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)

set_target_properties(abacus PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(abacus PRIVATE libabacus fmt::fmt)

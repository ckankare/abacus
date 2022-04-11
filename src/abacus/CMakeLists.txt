add_library(libabacus INTERFACE)

target_compile_features(libabacus INTERFACE cxx_std_20)
target_compile_options(libabacus INTERFACE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
          -Wall -Wconversion -Wextra -Wpedantic>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)

target_include_directories(libabacus
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # PRIVATE
    #     ${CMAKE_CURRENT_SOURCE_DIR}/include/common
)

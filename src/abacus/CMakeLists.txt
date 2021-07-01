add_library(libabacus INTERFACE)
target_compile_features(libabacus INTERFACE cxx_std_20)

target_include_directories(libabacus
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # PRIVATE
    #     ${CMAKE_CURRENT_SOURCE_DIR}/include/common
)

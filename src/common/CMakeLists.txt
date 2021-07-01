add_library(libcommon INTERFACE)
target_compile_features(libcommon INTERFACE cxx_std_20)

target_include_directories(libcommon
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # PRIVATE
    #     ${CMAKE_CURRENT_SOURCE_DIR}/include/common
)

# set_target_properties(libcommon PROPERTIES CXX_EXTENSIONS OFF)

# target_include_directories(libcommon
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#     PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}/include/common
# )

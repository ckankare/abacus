set(SOURCES
    tests.cpp
    big_int.cpp
)

add_executable(tests ${SOURCES})
target_compile_features(tests PUBLIC cxx_std_20)
set_target_properties(tests PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(tests PRIVATE libabacus fmt::fmt Catch2::Catch2)

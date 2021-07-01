set(SOURCES
    main.cpp
)

add_subdirectory(common)
add_subdirectory(abacus)

add_executable(abacus ${SOURCES})
target_compile_features(abacus PUBLIC cxx_std_20)
set_target_properties(abacus PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(abacus PRIVATE libabacus fmt::fmt)

set(SOURCES
    src/lexer.cpp
    src/scanner.cpp
    src/token.cpp
)

add_library(libascript ${SOURCES})
target_compile_features(libascript PUBLIC cxx_std_20)
set_target_properties(libascript PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(libascript
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ascript
)

target_link_libraries(libascript fmt::fmt libcommon)

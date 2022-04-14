#pragma once

#include <cstddef>

namespace asc {

struct SourceLocation {
    std::size_t start;
    std::size_t end;
};

struct position_t {
    std::size_t pos;
};

struct character_t {
    position_t pos;
    char character;
};

} // namespace asc

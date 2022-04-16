#pragma once

#include <cstddef>

namespace asc {

struct SourceLocation {
    std::size_t start{0};
    std::size_t end{0};
};

struct position_t {
    std::size_t pos;
};

struct character_t {
    position_t pos;
    char character;
};

} // namespace asc

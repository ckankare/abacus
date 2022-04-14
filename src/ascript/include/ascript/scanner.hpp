#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "source_location.hpp"

namespace asc {

class Scanner {
public:
    explicit Scanner(std::string_view src) : m_src(src), m_next(0u) {}
    character_t next();
    std::optional<character_t> next_if(char character);
    bool next_is(char character) const;
    bool next_is(const std::string_view str) const;
    std::optional<char> peek() const;
    std::optional<std::string_view> peek(std::size_t count) const;
    bool has_next() const;

private:
    std::string_view m_src;
    std::size_t m_next;
};
} // namespace asc

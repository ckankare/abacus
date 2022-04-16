#include "token.hpp"

namespace asc {
std::ostream& operator<<(std::ostream& os, const Token& value) {
    if (value.value().empty()) {
        os << fmt::format("[{}]", value.type());
    } else {
        os << fmt::format("[{}] \"{}\"", value.type(), value.value());
    }
    return os;
}
} // namespace asc

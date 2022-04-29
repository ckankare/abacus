#pragma once

#include <functional>
#include <vector>

#include "value.hpp"

namespace asc {

class Function {
public:
    virtual Value call(std::vector<Value> arguments) = 0;
    virtual ~Function() = default;
};

class NativeFunction final : public Function {
public:
    using native_function_t = Value(std::vector<Value>);
    explicit NativeFunction(native_function_t function) : m_function(function) {}
    Value call(std::vector<Value> arguments) override { return m_function(std::move(arguments)); };

private:
    std::function<Value(std::vector<Value>)> m_function;
};

} // namespace asc

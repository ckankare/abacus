#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "function.hpp"
#include "value.hpp"

namespace asc {

class Scope {
public:
    const Value* find_variable(std::string_view name) const {
        auto it = m_variables.find(name);
        if (it == m_variables.end()) {
            return nullptr;
        }
        return &it->second;
    };

    Value* find_variable(std::string_view name) {
        auto it = m_variables.find(name);
        if (it == m_variables.end()) {
            return nullptr;
        }
        return &it->second;
    };

    const Function* find_function(std::string_view name) const {
        auto it = m_functions.find(name);
        if (it == m_functions.end()) {
            return nullptr;
        }
        return it->second.get();
    };

    Function* find_function(std::string_view name) {
        auto it = m_functions.find(name);
        if (it == m_functions.end()) {
            return nullptr;
        }
        return it->second.get();
    };

    void define(std::string_view name, Value value) { m_variables[std::string(name)] = std::move(value); }
    void define(std::string_view name, std::unique_ptr<Function> function) {
        m_functions[std::string(name)] = std::move(function);
    }

private:
    struct string_hash {
        using is_transparent = void;
        [[nodiscard]] size_t operator()(std::string_view txt) const { return std::hash<std::string_view>{}(txt); }
        [[nodiscard]] size_t operator()(const std::string& txt) const { return std::hash<std::string>{}(txt); }
    };
    std::unordered_map<std::string, Value, string_hash, std::equal_to<>> m_variables;
    std::unordered_map<std::string, std::unique_ptr<Function>, string_hash, std::equal_to<>> m_functions;
};

class Interpreter {
public:
    Value* find_variable(std::string_view name) {
        for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
            if (auto* value = it->find_variable(name); value) {
                return value;
            }
        }
        return nullptr;
    }

    Function* find_function(std::string_view name) {
        for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
            if (auto* function = it->find_function(name); function) {
                return function;
            }
        }
        return nullptr;
    }

    void push() { m_scopes.push_back(Scope{}); }

    void define_variable(std::string_view name, Value value) {
        auto& top = m_scopes.front();
        top.define(name, std::move(value));
    }
    void define_function(std::string_view name, std::unique_ptr<Function> function) {
        auto& top = m_scopes.front();
        top.define(name, std::move(function));
    }

private:
    std::vector<Scope> m_scopes;
};
} // namespace asc

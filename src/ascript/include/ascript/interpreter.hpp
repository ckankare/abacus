#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "value.hpp"

namespace asc {

class Scope {
public:
    const Value* operator[](std::string_view name) const {
        // auto it = std::find(m_variables.begin(), m_variables.end(), name);
        auto it = m_variables.find(name);
        if (it == m_variables.end()) {
            return nullptr;
        }
        return &it->second;
    };

    Value* operator[](std::string_view name) {
        // auto it = std::find(m_variables.begin(), m_variables.end(), name);
        auto it = m_variables.find(name);
        if (it == m_variables.end()) {
            return nullptr;
        }
        return &it->second;
    };

    void define(std::string_view name, Value value) { m_variables[std::string(name)] = std::move(value); }

private:
    struct string_hash {
        using is_transparent = void;
        [[nodiscard]] size_t operator()(std::string_view txt) const { return std::hash<std::string_view>{}(txt); }
        [[nodiscard]] size_t operator()(const std::string& txt) const { return std::hash<std::string>{}(txt); }
    };
    std::unordered_map<std::string, Value, string_hash, std::equal_to<>> m_variables;
};

class Interpreter {
public:
    Value* find_variable(std::string_view name) {
        for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
            if (auto value = (*it)[name]; value) {
                return value;
            }
        }
        return nullptr;
    }

    void push() { m_scopes.push_back(Scope{}); }

    void define_variable(std::string_view name, Value value) {
        auto& top = m_scopes.front();
        top.define(name, std::move(value));
    }

private:
    std::vector<Scope> m_scopes;
};
} // namespace asc

// structes.h
// 结构体定义和实现
#pragma once

#include "heads.h"

// 定义 MouseButton、MouseMode 和 MoveMode 枚举
enum class MouseButton { LEFT, MIDDLE, RIGHT };
enum class MouseMode { PUT, PULL, DEFAULT };
enum class MoveMode { GLOBAL, SINGAL };

// 封装变量类型
struct Value {
    std::any value;
    Value() = default;
    explicit Value(std::any val) : value(std::move(val)) {}
};


// 使用全局变量
extern std::unordered_map<std::string, Value> variables;

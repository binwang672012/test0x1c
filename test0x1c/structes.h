// structes.h
// �ṹ�嶨���ʵ��
#pragma once

#include "heads.h"

// ���� MouseButton��MouseMode �� MoveMode ö��
enum class MouseButton { LEFT, MIDDLE, RIGHT };
enum class MouseMode { PUT, PULL, DEFAULT };
enum class MoveMode { GLOBAL, SINGAL };

// ��װ��������
struct Value {
    std::any value;
    Value() = default;
    explicit Value(std::any val) : value(std::move(val)) {}
};


// ʹ��ȫ�ֱ���
extern std::unordered_map<std::string, Value> variables;

// virtual_command.h
// 抽象指令的实现和所有指令的接口
#pragma once
#include "heads.h"
#include "structes.h"
#include "simulation.h"

template <typename T> T get_value(const std::any& any_value);
bool is_valid_variable_name(const std::string& name);
std::string parse_variable_reference(const std::string& token);
void print_usage(const std::string& cmd);
void create_variable(const std::string& name, const std::string& defvalue = "");
template <typename T>
void apply_operation(const std::string& name, const std::string& opr, const T& right_value);
void operation(const std::string& name, const std::string& opr, const std::string& right);
void execute_command(const std::string& command);
std::ostream& operator<<(std::ostream& os, const Value& val);

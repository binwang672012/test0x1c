// virtual_command.cpp
// virtual_command.h的实现
#include "virtual_command.h"

// 重载 << 操作符
// 你问我为什么放这里？只能放这里
std::ostream& operator<<(std::ostream& os, const Value& val) {
    if (val.value.type() == typeid(int)) {
        os << std::any_cast<int>(val.value);
    }
    else if (val.value.type() == typeid(float)) {
        os << std::any_cast<float>(val.value);
    }
    else if (val.value.type() == typeid(std::string)) {
        os << std::any_cast<std::string>(val.value);
    }
    else {
        os << "Unsupported type: " << val.value.type().name() << " or " << val.value.type().raw_name() << "(in cpp)";
    }
    return os;
}


// 辅助函数：获取 std::any 中的值类型并转换
template <typename T>
T get_value(const std::any& any_value) {
    if (any_value.type() == typeid(T)) {
        return std::any_cast<T>(any_value);
    }
    throw std::bad_any_cast();
}

// 合法变量名检查函数
bool is_valid_variable_name(const std::string& name) {
    static const std::regex valid_name_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
    return std::regex_match(name, valid_name_regex);
}

// 解析变量引用
std::string parse_variable_reference(const std::string& token) {
    if (token.size() > 2 && token.front() == '`' && token.back() == '`') {
        return token.substr(1, token.size() - 2);
    }
    return token;
}

// 打印用法
void print_usage(const std::string& cmd) {
    if (cmd == "CLICK") {
        std::cerr << "Usage: CLICK key(LEFT|MIDDLE|RIGHT) mode[(PUT|PULL|!) = !]" << std::endl;
    }
    else if (cmd == "KEY") {
        std::cerr << "Usage: KEY key{string} mode[(PUT|PULL|!) = !]" << std::endl;
    }
    else if (cmd == "MPOS") {
        std::cerr << "Usage: MPOS x{uint} y{uint} mode[(GLOBAL|SINGAL) = GLOBAL]" << std::endl;
    }
    else if (cmd == "VAL") {
        std::cerr << "Usage: VAL name{string} defvalue[{string}=]" << std::endl;
    }
    else if (cmd == "OPR") {
        std::cerr << "Usage: OPR name{string} opr{string} right{string|val}" << std::endl;
    }
    else if (cmd == "FOR") {
        std::cerr << "Usage: FOR name{string} beg{int} end{int} step{int}" << std::endl;
    }
    else if (cmd == "WHILE") {
        std::cerr << "Usage: WHILE name{string}" << std::endl;
    }
    else if (cmd == "DEBUG") {
        std::cerr << "Usage: DEBUG PRINT" << std::endl;
    }
    else {
        std::cerr << "Unknown command: " << cmd << std::endl;
    }
}

// 创建变量函数
void create_variable(const std::string& name, const std::string& defvalue) {
    if (!is_valid_variable_name(name)) {
        std::cerr << "Invalid variable name: " << name << std::endl;
        return;
    }

    if (!defvalue.empty()) {
        std::string ref_value = parse_variable_reference(defvalue);
        if (variables.find(ref_value) != variables.end()) {
            variables[name] = variables[ref_value];
        }
        else if (isdigit(defvalue[0]) || (defvalue[0] == '-' && isdigit(defvalue[1]))) {
            if (defvalue.find('.') != std::string::npos) {
                variables[name] = Value(std::stof(defvalue));
            }
            else {
                variables[name] = Value(std::stoi(defvalue));
            }
        }
        else {
            variables[name] = Value(defvalue);
        }
    }
    else {
        variables[name] = Value();
    }
}

// 操作变量函数模板
template <typename T>
void apply_operation(const std::string& name, const std::string& opr, const T& right_value) {
    if constexpr (std::is_arithmetic_v<T>) {
        if (opr == "+") {
            variables[name].value = get_value<T>(variables[name].value) + right_value;
        }
        else if (opr == "-") {
            variables[name].value = get_value<T>(variables[name].value) - right_value;
        }
        else if (opr == "*") {
            variables[name].value = get_value<T>(variables[name].value) * right_value;
        }
        else if (opr == "/") {
            if (right_value == 0) {
                std::cerr << "Error: Division by zero" << std::endl;
                return;
            }
            variables[name].value = get_value<T>(variables[name].value) / right_value;
        }
        else {
            std::cerr << "Unsupported operation: " << opr << std::endl;
        }
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        if (opr == "+") {
            variables[name].value = get_value<T>(variables[name].value) + right_value;
        }
        else {
            std::cerr << "Unsupported operation for strings: " << opr << std::endl;
        }
    }
}

// 操作变量函数
void operation(const std::string& name, const std::string& opr, const std::string& right) {
    if (variables.find(name) != variables.end()) {
        try {
            std::string ref_right = parse_variable_reference(right);
            Value right_value;
            if (variables.find(ref_right) != variables.end()) {
                right_value = variables[ref_right];
            }
            else if (isdigit(right[0]) || (right[0] == '-' && isdigit(right[1]))) {
                if (right.find('.') != std::string::npos) {
                    right_value = Value(std::stof(right));
                }
                else {
                    right_value = Value(std::stoi(right));
                }
            }
            else {
                right_value = Value(right);
            }

            if (opr == "=") {
                variables[name] = right_value;
            }
            else if (variables[name].value.type() == typeid(int) && right_value.value.type() == typeid(int)) {
                apply_operation(name, opr, get_value<int>(right_value.value));
            }
            else if (variables[name].value.type() == typeid(float) && right_value.value.type() == typeid(float)) {
                apply_operation(name, opr, get_value<float>(right_value.value));
            }
            else if (variables[name].value.type() == typeid(std::string) && right_value.value.type() == typeid(std::string)) {
                apply_operation(name, opr, get_value<std::string>(right_value.value));
            }
            else {
                std::cerr << "Type mismatch or unsupported type for operation: " << opr << std::endl;
            }
        }
        catch (const std::bad_any_cast& e) {
            std::cerr << "Bad any_cast: " << e.what() << '\n';
        }
    }
    else {
        std::cerr << "Variable not found: " << name << std::endl;
    }
}

// 解析并执行命令的函数
void execute_command(const std::string& command) {
    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        std::cerr << "No command provided" << std::endl;
        return;
    }

    const std::string& cmd = tokens[0];

    try {
        if (cmd == "CLICK") {
            if (tokens.size() < 2) {
                std::cerr << "Command needs 1 or more argument(s)." << std::endl;
                print_usage(cmd);
                return;
            }
            MouseButton button = (tokens[1] == "LEFT") ? MouseButton::LEFT :
                (tokens[1] == "MIDDLE") ? MouseButton::MIDDLE :
                (tokens[1] == "RIGHT") ? MouseButton::RIGHT :
                throw std::invalid_argument("Invalid mouse button");
            MouseMode mode = (tokens.size() == 3 && tokens[2] == "PUT") ? MouseMode::PUT :
                (tokens.size() == 3 && tokens[2] == "PULL") ? MouseMode::PULL :
                MouseMode::DEFAULT;
            click(button, mode);
        }
        else if (cmd == "KEY") {
            if (tokens.size() < 2) {
                std::cerr << "Command needs 1 or more argument(s)." << std::endl;
                print_usage(cmd);
                return;
            }
            std::string key_str = tokens[1];
            MouseMode mode = (tokens.size() == 3 && tokens[2] == "PUT") ? MouseMode::PUT :
                (tokens.size() == 3 && tokens[2] == "PULL") ? MouseMode::PULL :
                MouseMode::DEFAULT;
            key(key_str, mode);
        }
        else if (cmd == "MPOS") {
            if (tokens.size() < 3) {
                std::cerr << "Command needs 2 or more argument(s)." << std::endl;
                print_usage(cmd);
                return;
            }
            std::string x_str = parse_variable_reference(tokens[1]);
            std::string y_str = parse_variable_reference(tokens[2]);
            unsigned int x = (variables.find(x_str) != variables.end()) ? get_value<int>(variables[x_str].value) : std::stoul(x_str);
            unsigned int y = (variables.find(y_str) != variables.end()) ? get_value<int>(variables[y_str].value) : std::stoul(y_str);
            MoveMode mode = (tokens.size() == 4 && tokens[3] == "SINGAL") ? MoveMode::SINGAL : MoveMode::GLOBAL;
            move(x, y, mode);
        }
        else if (cmd == "VAL") {
            if (tokens.size() < 2) {
                std::cerr << "Command needs 1 or more argument(s)." << std::endl;
                print_usage(cmd);
                return;
            }
            std::string name = tokens[1];
            std::string defvalue = (tokens.size() == 3) ? tokens[2] : "";
            create_variable(name, defvalue);
        }
        else if (cmd == "OPR") {
            if (tokens.size() < 4) {
                std::cerr << "Command needs 3 or more argument(s)." << std::endl;
                print_usage(cmd);
                return;
            }
            std::string name = tokens[1];
            std::string opr = tokens[2];
            std::string right = tokens[3];
            operation(name, opr, right);
        }
        else if (cmd == "FOR") {
            if (tokens.size() != 5) {
                std::cerr << "Command needs exactly 4 argument(s)." << std::endl;
                print_usage(cmd);
                return;
            }
            std::string name = tokens[1];
            int beg = std::stoi(tokens[2]);
            int end = std::stoi(tokens[3]);
            int step = std::stoi(tokens[4]);
            // for_loop(name, beg, end, step);
        }
        else if (cmd == "WHILE") {
            if (tokens.size() != 2) {
                std::cerr << "Command needs exactly 1 argument(s)." << std::endl;
                print_usage(cmd);
                return;
            }
            std::string name = tokens[1];
            // while_loop(name);
        }
        else if (cmd == "DEBUG" && tokens.size() == 2 && tokens[1] == "PRINT") {
            for (const auto& var : variables) {
                std::cout << var.first << ": " << var.second << std::endl;
            }
        }
        else {
            std::cerr << "Unknown command: " << cmd << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error processing command: " << e.what() << std::endl;
        print_usage(cmd);
    }
}

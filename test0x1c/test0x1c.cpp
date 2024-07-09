// test0x1c.cpp
// 项目别叫这个名字吧
// 用MSVC编译!!!!!!!!!
// USE MSVC TO COMPLITE!!!!!!!!!!
#include <windows.h>
#include "heads.h"
#include "virtual_command.h"
#include "structes.h"

std::unordered_map<std::string, Value> variables;

int main() {
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "EXIT") break;
        execute_command(command);
    }

    return 0;
}

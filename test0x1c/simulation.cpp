// simulation.cpp
// simulation.h的实现
#include "simulation.h"

// 模拟鼠标点击
void click(MouseButton button, MouseMode mode) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    switch (button) {
    case MouseButton::LEFT:
        if (mode == MouseMode::PUT || mode == MouseMode::DEFAULT) {
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(INPUT));
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        else if (mode == MouseMode::PULL) {
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        break;
    case MouseButton::MIDDLE:
        if (mode == MouseMode::PUT || mode == MouseMode::DEFAULT) {
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            SendInput(1, &input, sizeof(INPUT));
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        else if (mode == MouseMode::PULL) {
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        break;
    case MouseButton::RIGHT:
        if (mode == MouseMode::PUT || mode == MouseMode::DEFAULT) {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            SendInput(1, &input, sizeof(INPUT));
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        else if (mode == MouseMode::PULL) {
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        break;
    default:
        std::cerr << "Unsupported mouse button" << std::endl;
        break;
    }
}

// 模拟键盘按键
void key(const std::string& key_str, MouseMode mode) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;

    try {
        if (key_str.find("VK-") == 0) {
            int vk_code = std::stoi(key_str.substr(3));
            input.ki.wVk = vk_code;
        }
        else {
            input.ki.wVk = VkKeyScan(key_str[0]);
        }
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Invalid key code: " << key_str << std::endl;
        return;
    }
    // 这里的不同模式没啥用
    if (mode == MouseMode::PUT) {
        input.ki.dwFlags = 0;  // Key press
        SendInput(1, &input, sizeof(INPUT));
    }
    else if (mode == MouseMode::PULL) {
        input.ki.dwFlags = KEYEVENTF_KEYUP;  // Key release
        SendInput(1, &input, sizeof(INPUT));
    }
    else if (mode == MouseMode::DEFAULT) {
        input.ki.dwFlags = 0;  // Key press
        SendInput(1, &input, sizeof(INPUT));
        input.ki.dwFlags = KEYEVENTF_KEYUP;  // Key release
        SendInput(1, &input, sizeof(INPUT));
    }
    else {
        std::cerr << "Unsupported key mode" << std::endl;
    }
}

// 模拟鼠标移动
void move(unsigned int x, unsigned int y, MoveMode mode, int screen_id) {
    if (mode == MoveMode::GLOBAL) {
        SetCursorPos(x, y);
    }
    else {
        // For simplicity, we'll treat SINGAL mode as similar to GLOBAL in this example.
        SetCursorPos(x, y);
    }
}

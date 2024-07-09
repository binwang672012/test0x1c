// simulation.h
// 古希腊掌管模拟的神
#pragma once
#include "heads.h"
#include "structes.h"
void click(MouseButton button, MouseMode mode);
void key(const std::string& key_str, MouseMode mode);
void move(unsigned int x, unsigned int y, MoveMode mode, int screen_id = 0);

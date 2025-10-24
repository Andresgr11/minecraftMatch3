#pragma once
#include <string>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
const std::string WINDOW_TITLE = "Minercaft Match-3";
constexpr int FPS = 60;
constexpr int BOARD_ROWS = 8;
constexpr int BOARD_COLS = 8;
constexpr int GEM_TYPE_QUANTITY = 5;
constexpr int GEM_SIDE_SIZE = 38;
constexpr int CELL_SIDE_SIZE = 40;
constexpr float GEM_MOVE_SPEED = 800.f;
constexpr float GEM_FADE_SPEED = 3.f;
constexpr int BOARD_X_START = 440;
constexpr int BOARD_Y_START = 240;
constexpr int BOARD_X_END = BOARD_X_START + BOARD_ROWS * CELL_SIDE_SIZE;
constexpr int BOARD_Y_END = BOARD_Y_START + BOARD_COLS * CELL_SIDE_SIZE;
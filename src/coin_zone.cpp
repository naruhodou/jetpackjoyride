#include "coinzone.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL

Coin_Zone :: Coin_Zone(float start_x, float start_y, float length, float width) {
    // Initialization of values
    this -> start_x = start_x;
    this -> start_y = start_y;
    this -> length = length;
    this -> width = width;
}
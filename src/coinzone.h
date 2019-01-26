#include "main.h"

#ifndef COIN_ZONE_H
#define COIN_ZONE_H
#define GLM_ENABLE_EXPERIMENTAL
class Coin_Zone {
public:
    Coin_Zone() {}
    Coin_Zone(float start_x, float start_y, float length, float width);
    float start_x, start_y, length, width;
private:
    VAO *object;
};

#endif // BALL_H
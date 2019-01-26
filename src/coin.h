#include "main.h"

#ifndef COIN_H
#define COIN_H
#define GLM_ENABLE_EXPERIMENTAL
class Coin {
public:
    Coin() {}
    Coin(float radius, float x, float y, color_t color);
    float rotation;
    glm::vec3 position;
    void draw(glm::mat4 VP);
    bounding_box_t coin;
    bool isdraw;
private:
    VAO *object;
};
#endif // BALL_H
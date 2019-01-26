#include "main.h"

#ifndef ENEMY1_H
#define ENEMY1_H
#define GLM_ENABLE_EXPERIMENTAL
class Enemy1 {
public:
    Enemy1() {}
    Enemy1(float x, float y, float length, float delta, bool ismove, float angle, color_t color);
    glm::vec3 position;
    float length;
    bool ismove;
    float rotation;
    bounding_box_t checker;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // ENEMY1_H

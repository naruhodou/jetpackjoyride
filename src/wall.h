#include "main.h"

#ifndef WALL_H
#define WALL_H
#define GLM_ENABLE_EXPERIMENTAL
class Wall {
public:
    Wall() {}
    Wall(float x, float y, color_t color, float width, float length, float start_coordinate, float start_coordinatey);
    float rotation;
    glm::vec3 position;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // BALL_H
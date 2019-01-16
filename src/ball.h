#include "main.h"

#ifndef BALL_H
#define BALL_H
#define GLM_ENABLE_EXPERIMENTAL
class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int sn);
    void horizontal_movement(bool flag);
    void vertical_movement(bool flag);
    double speed;
    bool moveY = false;
    //ball velocity components
    double vx, vy;
    //ball acceleration
    double ay;
    bool keypressx, keypressy;
private:
    VAO *object;
};

#endif // BALL_H

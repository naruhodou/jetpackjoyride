#include "ball.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Ball::Ball(float x, float y, color_t color) {
    // Initialization of values
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this -> vx = 0;
    this -> vy = 0;
    this -> score = 0;
    this -> ay = -10;
    this -> keypressx = false;
    this -> keypressy = false;
    speed = 0.05000000;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -0.5f,-0.5f,-0.5f, // triangle 1 : begin
        -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, // triangle 1 : end
        0.5f, 0.5f,-0.5f, // triangle 2 : begin
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f, // triangle 2 : end
        0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f, 0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        0.5f,-0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f, 0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f,-0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f,-0.5f, 0.5f
    };
    this -> player.width = this -> player.height = 1.0;
    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we want to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick(int motion_type) {
    if(this -> keypressx)
    {
        if(motion_type == 1)
            this -> position.x += speed;
        else if(motion_type == -1)
            this -> position.x -= speed;
    }
    if(this -> keypressy)
    {
        if(motion_type == 2 && this -> position.y <= window_size - 0.5)
        {
            this -> position.y += speed;
            this -> vy = speed;
        }
    }
    else
    {
        double displacement = this -> vy / 60 + this -> ay / 7200;
        this -> position.y += displacement;
        if(this -> position.y < 0)
            this -> position.y = 0;
        if(this -> position.y > 0)
            this -> vy += this -> ay / 60;
        else
            this -> vy = 0;
    }
    this -> player.x = this -> position.x;
    this -> player.y = this -> position.y;
}

void Ball :: horizontal_movement(bool flag)
{
    this -> keypressx = flag;
}
void Ball :: vertical_movement(bool flag)
{
    this -> keypressy = flag;
}

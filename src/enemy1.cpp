#include "enemy1.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Enemy1::Enemy1(float x, float y, float length, float delta, bool ismove, float angle, color_t color) {
    // Initialization of values
    this->position = glm::vec3(x, y, -1);
    this -> rotation = angle;
    this -> length = length;
    this -> ismove = ismove;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[] = {
        -length / 2, 0, 0,
        length / 2, 0, 0,
        length / 2, delta, 0,
        length / 2, delta, 0,
        -length / 2, delta, 0,
        -length / 2, 0, 0
    }; 
    checker.width = 2 * length;
    checker.height = 2 * delta;
    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Enemy1::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we want to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    checker.x = this -> position.x;
    checker.y = this -> position.y;
}

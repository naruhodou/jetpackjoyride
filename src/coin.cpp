#include "coin.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Coin::Coin(double radius, double x, double y, color_t color) {
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    this->position = glm::vec3(x, y, 0);
    this -> rotation = 0;
    this -> isdraw = true;
    GLfloat vertex_buffer_data[9 * 300];
    double angle = (2 * PI) / 300, theta = 0;
    for(int i = 0; i < 9 * 300; i++)
    {
        if(i % 3 == 2 || i % 9 < 3)
            vertex_buffer_data[i] = 0.0f;
        else if(i % 9 == 3)
            vertex_buffer_data[i] = radius * cos(theta);
        else if(i % 9 == 4)
            vertex_buffer_data[i] = radius * sin(theta);
        else if(i % 9 == 6)
        {
            theta += angle;
            vertex_buffer_data[i] = radius * cos(theta);
        }
        else
            vertex_buffer_data[i] = radius * sin(theta);
    }
    this->object = create3DObject(GL_TRIANGLES, 3 * 300, vertex_buffer_data, color, GL_FILL);
    this -> coin.x = x;
    this -> coin.y = y;
    this -> coin.width = this -> coin.height = (double)2 * radius;
}

void Coin::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we want to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= translate;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

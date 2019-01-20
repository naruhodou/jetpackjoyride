#include "wall.h"
#include "main.h"
#define GLM_ENABLE_EXPERIMENTAL
Wall::Wall(float x, float y, color_t color, float width, float length, float start_coordinatex, float start_coordinatey) {
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    this->position = glm::vec3(x, y, -1);
    this -> rotation = 0;
    GLfloat vertex_buffer_data[] = {
        start_coordinatex, start_coordinatey - width, 0,
        start_coordinatex + length, start_coordinatey - width, 0,
        start_coordinatex + length, start_coordinatey, 0,
        start_coordinatex + length, start_coordinatey, 0,
        start_coordinatex, start_coordinatey, 0,
        start_coordinatex, start_coordinatey - width, 0,
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Wall::draw(glm::mat4 VP) {
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

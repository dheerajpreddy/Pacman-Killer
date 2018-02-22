#include "magnet.h"
#include "main.h"
#include "stdlib.h"

Magnet::Magnet(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat g_vertex_buffer_data[]= {
      0.0f, 0.0f, 0.0f,
      0.5f, 0.0f, 0.0f,
      0, 1, 0,
      0, 1, 0,
      0.5, 1, 0,
      0.5, 0, 0,
      0.5, 1, 0,
      0.5, 0.8, 0,
      1, 0.8, 0,
      1, 0.8, 0,
      1, 1, 0,
      0.5, 1, 0,
      1, 1, 0,
      1, 0, 0,
      1.5, 0, 0,
      1.5, 0, 0,
      1.5, 1, 0,
      1, 1, 0,
    };

    this->object = create3DObject(GL_TRIANGLES, 18, g_vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Magnet::rand_position() {
    float x = (rand()%1000)/500;
    float y = (rand()%1000)/500;
    this->position = glm::vec3(x, y, 0);
}

void Magnet::tick() {
    this->position.x -= speed.x;
    this->position.y -= speed.y;
}

bounding_box_t Magnet::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 1, 1 };
    return bbox;
}

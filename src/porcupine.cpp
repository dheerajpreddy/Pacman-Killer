#include "porcupine.h"
#include "main.h"
#include "stdlib.h"

Porcupine::Porcupine(float x, float y, int n, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->n = n;
    GLfloat g_vertex_buffer_data[10000];
    int j = 0;
    float base = 0, height = 0.25f;
    for(int i = 0; i<n; i++, base+=0.25f) {
      g_vertex_buffer_data[j++] = base;
      g_vertex_buffer_data[j++] = 0;
      g_vertex_buffer_data[j++] = 0;
      g_vertex_buffer_data[j++] = base + 0.125f;
      g_vertex_buffer_data[j++] = height;
      g_vertex_buffer_data[j++] = 0;
      g_vertex_buffer_data[j++] = base + 0.25f;
      g_vertex_buffer_data[j++] = 0;
      g_vertex_buffer_data[j++] = 0;
    }
    this->object = create3DObject(GL_TRIANGLES, 3*n, g_vertex_buffer_data, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porcupine::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Porcupine::rand_position() {
    float x = (rand()%1000)/500;
    float y = (rand()%1000)/500;
    this->position = glm::vec3(x, y, 0);
}

void Porcupine::tick() {
    this->position.x -= speed.x;
    this->position.y -= speed.y;
}

bounding_box_t Porcupine::bounding_box() {
    float x = this->position.x, y = this->position.y;
    int n = this->n;
    bounding_box_t bbox = { x, y, n*0.25, 0.25 };
    return bbox;
}

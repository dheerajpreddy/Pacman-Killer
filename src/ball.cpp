#include "ball.h"
#include "main.h"
#include "stdlib.h"

Ball::Ball(float x, float y, float radius, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat g_vertex_buffer_data[100000];
    int j=0;
    int angle = 0;
    for (int i=0; i<1080; i++) {
        if(i%3==0){
            g_vertex_buffer_data[j++] = 0.0f;
            g_vertex_buffer_data[j++] = 0.0f;
            g_vertex_buffer_data[j++] = 0.0f;
        } else if(i%3==1) {
            g_vertex_buffer_data[j++] = radius*cos((angle*3.1428)/180);
            g_vertex_buffer_data[j++] = radius*sin((angle*3.1428)/180);
            g_vertex_buffer_data[j++] = 0.0f;
            angle+=1;
        } else {
            g_vertex_buffer_data[j++] = radius*cos((angle*3.1428)/180);
            g_vertex_buffer_data[j++] = radius*sin((angle*3.1428)/180);
            g_vertex_buffer_data[j++] = 0.0f;
        }
    }

    this->object = create3DObject(GL_TRIANGLES, 10000, g_vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::rand_position() {
    float x = (rand()%1000)/500;
    float y = (rand()%1000)/500;
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x -= speed.x;
    this->position.y -= speed.y;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.3, 0.3 };
    return bbox;
}

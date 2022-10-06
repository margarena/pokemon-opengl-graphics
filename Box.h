#ifndef BOX_H
#define BOX_H


#include "RigidBody.h"

class Drawable;

class Box : public RigidBody {
public:
    Drawable* box;
    float l;
    float w;
    glm::mat4 modelMatrix;

    Box(glm::vec3 pos, glm::vec3 vel, float width, float length, float mass);
    ~Box();

    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif
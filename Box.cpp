#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>

using namespace glm;

Box::Box(vec3 pos, vec3 vel, float width, float length, float mass)
    : RigidBody() {
    box = new Drawable("models/cube.obj");

    l = length;
    m = mass;
    x = pos;
    v = vel;
    P = m * v;
    w = width;

    if (l == 0) throw std::logic_error("Cube: length != 0");
    mat3 I = mat3(
        1.0f / 6 * mass * l * w, 0, 0,
        0, 1.0f / 6 * mass * l * w, 0,
        0, 0, 1.0f / 6 * mass * l * w);

  //  L = I * w;
    I_inv = inverse(I);
}

Box::~Box() {
    delete box;
}

void Box::draw(unsigned int drawable) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    box->bind();
    box->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
}

void Box::update(float t, float dt) {
    // numerical integration
    advanceState(t, dt);

    // compute model matrix
    mat4 scale = glm::scale(mat4(), vec3(l, l, w));
    mat4 tranlation = translate(mat4(), vec3(x.x, x.y, x.z));
#ifdef USE_QUATERNIONS
    mat4 rotation = mat4_cast(q);
#else
    mat4 rotation = mat4(R);
#endif
    modelMatrix = tranlation * rotation * scale;
}
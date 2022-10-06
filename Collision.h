#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

class Box;
class Sphere;
void handleBoxSphereCollision1(Box& box, Sphere& sphere);
void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2);
void handleBoxSphereCollision2(Box& box, Sphere& sphere);
void handleBoxSphereCollision3(Box& box, Sphere& sphere);
void handleBoxSphereCollision4(Box& box, Sphere& sphere);
void handleBoxSphereCollision5(Box& box, Sphere& sphere);
bool threeTimesTheCharm(int collisionCounter);
//void handleBoxSphereCollision6(Box& box, Sphere& sphere);


#endif

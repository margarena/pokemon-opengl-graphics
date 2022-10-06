#include "Collision.h"
#include "Box.h"
#include "Sphere.h"
#include <iostream>
using namespace glm;

int collisionCounter = 0;

//box1//
void handleBoxSphereCollision(Box& box, Sphere& sphere);
bool checkForBoxSphereCollision1(glm::vec3 &pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n);
//box2//
bool checkForBoxSphereCollision2(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n);
//box3//
bool checkForBoxSphereCollision3(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n);
//box4//
bool checkForBoxSphereCollision4(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n);
//box5//
bool checkForBoxSphereCollision5(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n);
//box//
//bool checkForBoxSphereCollision6(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n);

//3 times collision//
bool threeTimesTheCharm(int collisionCounter);

void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2);
bool checkForSphereSphereCollision(glm::vec3& pos1, const float& r1, glm::vec3& pos2, const float& r2, glm::vec3& n);


void handleBoxSphereCollision1(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision1(sphere.x, sphere.r, box.x, box.l, box.w, n)) {
        // Task 2b: define the velocity of the sphere after the collision
        //eswteriko ginomeno v*normal = probolh kai to afairw apo to v 2 fores
        sphere.v = sphere.v - n * glm::dot(sphere.v, n) * 1.9f;
        sphere.P = sphere.m * sphere.v;
        std::cout << "collision 1" << std::endl;
    }
}

void handleBoxSphereCollision2(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision2(sphere.x, sphere.r, box.x, box.l, box.w, n)) {
        // Task 2b: define the velocity of the sphere after the collision
        //eswteriko ginomeno v*normal = probolh kai to afairw apo to v 2 fores
        sphere.v = sphere.v - n * glm::dot(sphere.v, n) * 1.9f;
        sphere.P = sphere.m * sphere.v;
        std::cout << "collision 2" << std::endl;
    }
}

    
void handleBoxSphereCollision3(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision3(sphere.x, sphere.r, box.x, box.l, box.w, n)) {
        // Task 2b: define the velocity of the sphere after the collision
        //eswteriko ginomeno v*normal = probolh kai to afairw apo to v 2 fores
        sphere.v = sphere.v - n * glm::dot(sphere.v, n) * 1.9f;
        sphere.P = sphere.m * sphere.v;
        std::cout << "collision 3" << std::endl;
    }
}
    
void handleBoxSphereCollision4(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision4(sphere.x, sphere.r, box.x, box.l, box.w, n)) {
        // Task 2b: define the velocity of the sphere after the collision
        //eswteriko ginomeno v*normal = probolh kai to afairw apo to v 2 fores
        sphere.v = sphere.v - n * glm::dot(sphere.v, n) * 1.9f;
        sphere.P = sphere.m * sphere.v;
        std::cout << "collision 4" << std::endl;
    }
}

void handleBoxSphereCollision5(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision5(sphere.x, sphere.r, box.x, box.l, box.w, n)) {
        // Task 2b: define the velocity of the sphere after the collision
        //eswteriko ginomeno v*normal = probolh kai to afairw apo to v 2 fores
        sphere.v = sphere.v - n * glm::dot(sphere.v, n) * 1.9f;
        sphere.P = sphere.m * sphere.v;
        std::cout << "collision 5" << std::endl;
    }
}
/*
void handleBoxSphereCollision6(Box& box, Sphere& sphere) {
    vec3 n;
    if (checkForBoxSphereCollision6(sphere.x, sphere.r, box.x, box.l, box.w, n)) {
        // Task 2b: define the velocity of the sphere after the collision
        //eswteriko ginomeno v*normal = probolh kai to afairw apo to v 2 fores
        sphere.v = sphere.v - n * glm::dot(sphere.v, n) * 1.9f;
        sphere.P = sphere.m * sphere.v;
        std::cout << "collision 6" << std::endl;
    }

}
  */

void handleSphereSphereCollision(Sphere& sphere1, Sphere& sphere2) {
    vec3 n;
    if (checkForSphereSphereCollision(sphere1.x, sphere1.r, sphere2.x, sphere2.r, n)) {
        //xrhsimopoiw tis eksiswseis ths elastikhs kroushs//
        vec3 last_vel = sphere1.v;
        sphere1.v =( ((sphere1.m - sphere2.m) / (sphere1.m + sphere2.m)) * length(sphere1.v) + ((sphere2.m) / (sphere1.m +sphere2.m)) * length(sphere2.v) * 2.0f )* glm::reflect(normalize(sphere1.v), n);
        sphere2.v = (((sphere2.m - sphere1.m) / (sphere1.m + sphere2.m)) * length(sphere2.v) + ((sphere1.m) / (sphere1.m + sphere2.m)) * length(last_vel) * 2.0f) * -n ;

        sphere1.P = sphere1.m * sphere1.v;
        sphere2.P = sphere2.m * sphere2.v;
    }
}
/*
bool checkForBoxCollistion(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n) {
    if (pos1.x - r <= pos2.x - (l / 2.0) && pos1.z <= pos2.z + w/2.0 && pos1.z > pos2.z - w / 2.0) {
    
    }
}
*/
bool checkForBoxSphereCollision1(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n) {
    //box = box1//

    if (pos1.x - r <= pos2.x - (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
        //float dis = -(pos1.x - r) - l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(-1, 0, 0);
    }
   
    else if (pos1.x + r >= pos2.x + (l/2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
        //float dis = l - (pos1.x + r)-l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(1, 0, 0);
        
    }
   
    else if (pos1.y - r <= pos2.y - (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
       // float dis = l/2 - (pos1.y - r) ;
       // pos1 = pos1 + vec3(0, dis, 0);
        n = vec3(0, -1, 0);
    }
    
    
    else if ((pos1.z - r <= pos2.z - w / 2.0) && (pos1.x <= -4.0 || pos1.x >=  -1.0) ) {
        //correction
        //float dis =  (pos1.z - r) - l/2;
        //float dis = -(pos1.z - r);
       // pos1 = pos1 + vec3(0, 0, dis);


        n = vec3(0, 0, -1);
    }
    /*
    else if (pos1.z + r >= pos2.z + (l / 2.0) && pos1.z <= 3.0 && pos1.z > -6.0) {
        //correction
        float dis = l - (pos1.z + r)-l/2;
        pos1 = pos1 + vec3(0, 0, dis);

        n = vec3(0, 0, 1);
    }
    */
    else {
        return false;
    }
    
    return true;
}

bool checkForBoxSphereCollision2(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n) {
    //box = box2//
    if (pos1.x - r <= pos2.x - (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
       //float dis = -(pos1.x - r) - l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(-1, 0, 0);
    }
    else if (pos1.x + r >= pos2.x + (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
      // float dis = l - (pos1.x + r)-l/2;
       // pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(1, 0, 0);
    }
    else if (pos1.y - r <= ( pos2.y - (l / 2.0) ) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
        //float dis = l/2 -(pos1.y - r) ;
        //pos1 = pos1 + vec3(0, dis, 0);

        n = vec3(0, -1, 0);
    }
  
  
    else {
        return false;
    }

    return true;
}

bool checkForBoxSphereCollision3(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n) {
    //box = box3//
    if (pos1.x - r <= pos2.x - (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
        //float dis = -(pos1.x - r) - l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(-1, 0, 0);
    }
    else if (pos1.x + r >= pos2.x + (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
       // float dis = l - (pos1.x + r)-l/2;
       // pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(1, 0, 0);
    }
    else if (pos1.y - r <= pos2.y - (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
       // float dis = -(pos1.y - r);
       // pos1 = pos1 + vec3(0, dis, 0);

        n = vec3(0, -1, 0);
    }
   
    else if (pos1.z + r >= pos2.z + w/2.0 ) {
        //correction
      //  float dis = l - (pos1.z + r)-l/2;
     //   pos1 = pos1 + vec3(0, 0, dis);

        n = vec3(0, 0, 1);
    }
    
    else {
        return false;
    }

    return true;
}

bool checkForBoxSphereCollision4(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n) {
    //box = box4//
    if (pos1.x - r <= pos2.x - (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
        //float dis = -(pos1.x - r) - l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(-1, 0, 0);
    }
    else if (pos1.x + r >= pos2.x + (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
        //float dis = l - (pos1.x + r)-l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(1, 0, 0);
    }
    else if (pos1.y - r <= pos2.y - (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
        //float dis = l/2 -(pos1.y - r) ;
        //pos1 = pos1 + vec3(0, dis, 0);

        n = vec3(0, -1, 0);
    }
    else {
        return false;
    }

    return true;
}

bool checkForBoxSphereCollision5(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n) {
    //box = box5//
    if (pos1.x - r <= pos2.x - (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
        //float dis = -(pos1.x - r) - l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(-1, 0, 0);
    }
    else if (pos1.x + r >= pos2.x + (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
        //float dis = l - (pos1.x + r)-l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(1, 0, 0);
    }
    else if (pos1.y - r <= pos2.y - (l / 2.0) && pos1.z <= pos2.z + w / 2.0 && pos1.z > pos2.z - w / 2.0) {
        //correction
       // float dis = l/2 - (pos1.y - r) ;
       // pos1 = pos1 + vec3(0, dis, 0);
        n = vec3(0, -1, 0);
    }

    else if (pos1.z - r <= pos2.z - w / 2.0) {
        //correction
        //float dis = -(pos1.z - r)-l/2;
       //e pos1 = pos1 + vec3(0, 0, dis);

        n = vec3(0, 0, -1);
    }
   
    else {
        return false;
    }

    return true;
}
/*
bool checkForBoxSphereCollision6(glm::vec3& pos1, const float& r, glm::vec3& pos2, const float& l, const float& w, glm::vec3& n) {
    //box = box6//
    if (pos1.x - r <= pos2.x - (l / 2.0) && pos1.z > -15.0 && pos1.z < -6.0) {
        //correction
        //float dis = -(pos1.x - r) - l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(-1, 0, 0);
    }
    else if (pos1.x + r >= pos2.x + (l / 2.0) && pos1.z > -15.0 && pos1.z < -6.0) {
        //correction
        //float dis = l - (pos1.x + r)-l/2;
        //pos1 = pos1 + vec3(dis, 0, 0);

        n = vec3(1, 0, 0);
    }
    else if (pos1.y - r <= pos2.y - (l / 2.0) && pos1.z > -15.0 && pos1.z < -6.0) {
        //correction
        //float dis = l/2 -(pos1.y - r) ;
        //pos1 = pos1 + vec3(0, dis, 0);

        n = vec3(0, -1, 0);
    }
    else if (pos1.y + r >= pos2.y + (l / 2.0) && pos1.z > -15.0 && pos1.z < -6.0) {
        //correction
       // float dis = 2*l - (pos1.y + r) + l/2 ;
      //  pos1 = pos1 + vec3(0, dis, 0);

        n = vec3(0, 1, 0);
    }
   
    
    else {
        return false;
    }

    return true;
}
 */

bool checkForSphereSphereCollision(glm::vec3& pos1, const float& r1, glm::vec3& pos2, const float& r2, glm::vec3& n) {
    if (distance(pos1, pos2) <= (r1 + r2)) {
        n = normalize(pos1 - pos2);
        return true;
    }
    else {
        return false;
    }
}

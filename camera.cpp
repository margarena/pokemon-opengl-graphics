#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

using namespace glm;

Camera::Camera(GLFWwindow* window) : window(window) {
    //position = vec3(0, 8, 30);
    //oria ground:
    //x->-7.5, 7.5
    //y->8, idc
    //z->, 20
    position = vec3(6.5, 3.5, 17);
    horizontalAngle = 3.14f;
    verticalAngle = 0.0f;
    FoV = 45.0f;
    speed = 5.0f;
    mouseSpeed = 0.001f;
    fovSpeed = 2.0f;
    
}

void Camera::update() {
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, width / 2, height / 2);

    // Task 5.3: Compute new horizontal and vertical angles, given windows size
    // and cursor position
    horizontalAngle += mouseSpeed * float(width / 2 - xPos);
    verticalAngle += mouseSpeed * float(height / 2 - yPos);

    // Task 5.4: right and up vectors of the camera coordinate system
    // use spherical coordinates
      direction = vec3(
       cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
       cos(verticalAngle) * cos(horizontalAngle)
    );
     
    // Right vector
    vec3 right(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    vec3 up = cross(right, direction);

    // Task 5.5: update camera position using the direction/right vectors
    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime  * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime  * speed;
        
        if (position.z > 20) {
            position.z = 20.0;
        }
        if (position.x > 7.5) {
            position.x = 7.5;
        }
        if (position.x < -7.5) {
            position.x = -7.5;
        }
        if (position.y < 0.0) {
            position.y = 0.0;
        }
        if (position.y > 10.0) {
            position.y = 10.0;
        }
       
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * speed;
        
       if (position.z>20) {
           position.z = 20.0;
       }
       if (position.x > 7.5) {
           position.x = 7.5;
       }
       if (position.x < -7.5) {
           position.x = -7.5;
       }
       if (position.y < 0.0) {
           position.y = 0.0;
       }
       if (position.y > 10.0) {
           position.y = 10.0;
       }
       
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
        
      if (position.z>20) {
          position.z = 20.0;
      }
      if (position.x > 7.5) {
          position.x = 7.5;
      }
      if (position.x < -7.5) {
          position.x = -7.5;
      }
      if (position.y < 0.0) {
          position.y = 0.0;
      }
      if (position.y > 10.0) {
          position.y = 10.0;
      }
      
    }
    
    // move up
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        position += up * deltaTime * speed;
    }
    //move down
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        position -= up * deltaTime * speed;
    }
    
    // Task 5.6: handle zoom in/out effects
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        FoV -= fovSpeed * deltaTime * 100.0;
        if (FoV < 0.0) {
            FoV = 1.0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        FoV += fovSpeed * deltaTime * 100.0;
        if (FoV > 90.0) {
            FoV = 90.0;
        }
    }

    // Task 5.7: construct projection and view matrices
    projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    viewMatrix = lookAt(
        position,
        position + direction,
        up
    );
    //*/

    // Homework XX: perform orthographic projection

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}
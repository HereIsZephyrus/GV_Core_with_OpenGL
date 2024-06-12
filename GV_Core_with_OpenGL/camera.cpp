//
//  Camera.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/12/24.
//

#include "camera.hpp"
/*
Camera2D::Camera2D(GLfloat zoom):
    position (glm::vec3(0.0f, 0.0f, 0.0f)),
    movementSpeed(cm::SPEED),
    zoom(zoom),
    worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    front(glm::vec3(0.0f, 0.0f, -1.0f))
    {this->updateCameraVectors();}

void Camera::ProcessKeyCommand(CameraMovement direction, GLfloat deltaTime){
    
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
     
    GLfloat velocity = this->MovementSpeed * deltaTime;
    switch (direction){
        case CameraMovement::left:
            this->position -= this->right * velocity;
            break;
        case CameraMovement::right:
            this->position += this->right * velocity;
            break;
        case CameraMovement::up:
            this->position += this->up * velocity;
            break;
        case CameraMovement::down:
            this -> position -= this -> up * velocity;
            break;
        default:
            std::cerr<<"CAMERAMOVEMENT::non exist case!"<<std::endl;
    }
}
*/

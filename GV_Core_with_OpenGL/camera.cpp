//
//  Camera.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/12/24.
//

#include "camera.hpp"
#include "commander.hpp"
#include "window.hpp"

void Camera2D::processKeyboard(GLFWwindow* window, float deltaTime) {
    Records& record = Records::getState();
    const float cameraSpeed = 400.0f * deltaTime;
    if (record.pressAlt || record.pressCtrl|| record.pressShift)//ignore function input
        return;
    if (record.keyRecord[GLFW_KEY_W])
        position.y += cameraSpeed;
    if (record.keyRecord[GLFW_KEY_S])
        position.y -= cameraSpeed;
    if (record.keyRecord[GLFW_KEY_A])
        position.x -= cameraSpeed;
    if (record.keyRecord[GLFW_KEY_D])
        position.x += cameraSpeed;
    updateViewMatrix();
}
void Camera2D::zoomInOut(float yOffset) {
    zoom -= yOffset * 0.1f;
    if (zoom < 0.01f)
        zoom = 0.01f;
    if (zoom > 10.0f)
        zoom = 10.0f;
    updateProjectionMatrix();
    updateViewMatrix();
}
Camera2D::Camera2D() : position(0.0f, 0.0f), zoom(1.0f){
    WindowParas& windowPara = WindowParas::getInstance();
    screenWidth = windowPara.WINDOW_WIDTH;
    screenHeight = windowPara.WINDOW_HEIGHT;
    updateProjectionMatrix();
    updateViewMatrix();
}
void Camera2D::loadSavedPara(const CameraPara2D* para){
    this->position = para->position;
    this->zoom = para->zoom;
    this->screenWidth = para->screenWidth;
    this->screenHeight = para->screenHeight;
    this->projectionMatrix = para->projectionMatrix;
    this->viewMatrix = para->viewMatrix;
    updateProjectionMatrix();
    updateViewMatrix();
}

namespace cm {
const pCamera2D zeroCamera(new CameraPara2D(glm::vec2(0.0f, 0.0f),1.0f,
                                                   WindowParas::getInstance().WINDOW_WIDTH,WindowParas::getInstance().WINDOW_HEIGHT));
std::vector<pCamera2D> cameras;
}

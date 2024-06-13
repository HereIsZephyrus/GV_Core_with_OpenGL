//
//  Camera.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/12/24.
//

#ifndef camera_hpp
#define camera_hpp

#include <iostream>
#include <vector>
#include <memory>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct CameraPara2D{
    glm::vec2 position;
    float zoom;
    float screenWidth;
    float screenHeight;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    CameraPara2D(glm::vec2 position,float zoom,float screenWidth,float screenHeight){
        this->position = position;
        this->zoom = zoom;
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
    }
};
typedef std::unique_ptr<CameraPara2D> pCamera2D;
namespace cm {
extern const pCamera2D zeroCamera;
extern std::vector<pCamera2D> cameras;
}
class Camera2D {
public:
    static Camera2D& getView(){
        static Camera2D instance;
        return instance;
    }
    void processKeyboard(GLFWwindow* window, float deltaTime);
    void zoomInOut(float yOffset);
    void loadSavedPara(const CameraPara2D* para);
    glm::mat4 getProjectionMatrix() const {return projectionMatrix;}
    glm::mat4 getViewMatrix() const {return viewMatrix;}

private:
    Camera2D();
    glm::vec2 position;
    float zoom;
    float screenWidth;
    float screenHeight;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    void updateProjectionMatrix() {
        float left = -screenWidth / 2.0f * zoom;
        float right = screenWidth / 2.0f * zoom;
        float bottom = -screenHeight / 2.0f * zoom;
        float top = screenHeight / 2.0f * zoom;
        projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    }

    void updateViewMatrix() {
        viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
    }
};
#endif /* camera_hpp */

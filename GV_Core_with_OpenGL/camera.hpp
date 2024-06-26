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
    GLfloat screenWidth;
    GLfloat screenHeight;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    CameraPara2D(glm::vec2 position,float zoom,GLfloat screenWidth,GLfloat screenHeight){
        this->position = position;
        this->zoom = zoom;
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
    }
};
typedef std::unique_ptr<CameraPara2D> pCamera2D;
namespace cm {
extern pCamera2D zeroCamera;
extern std::vector<pCamera2D> cameras;
}
class Camera2D {
public:
    static Camera2D& getView(){
        static Camera2D instance;
        return instance;
    }
    void processKeyboard(GLFWwindow* window);
    void processScroll(GLFWwindow* window, double xoffset, double yoffset, bool pressCtrl, bool pressAlt);
    void zoomInOut(float yOffset);
    void loadSavedPara(const CameraPara2D* para);
    glm::mat4 getProjectionMatrix() const {return projectionMatrix;}
    glm::mat4 getViewMatrix() const {return viewMatrix;}
    float getZoom() const {return zoom;}
    glm::vec2 getPosition() const {return position;}
    GLfloat getCameraSpeed(GLfloat speed) const {return speed * deltaTime * zoom;}
    void setPosition(GLfloat x,GLfloat y){
        position.x = x; position.y = y;
    }
private:
    Camera2D();
    glm::vec2 position;
    float zoom;
    GLfloat screenWidth;
    GLfloat screenHeight;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    const GLfloat deltaTime = 0.016f; // 60FPS
    void updateProjectionMatrix();
    void updateViewMatrix() ;
};
#endif /* camera_hpp */

//
//  window.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/8/24.
//

#ifndef window_hpp
#define window_hpp
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

// build a singleton to manage all window vars
class WindowParas{
public:
    static WindowParas& getInstance(){
        static WindowParas instance;
        return instance;
    }
    WindowParas(const WindowParas&) = delete;
    void operator=(const WindowParas&) = delete;
    
    const GLint WINDOW_WIDTH = 800;
    const GLint WINDOW_HEIGHT = 600;
    int SCREEN_WIDTH,SCREEN_HEIGHT;
    
    float calcNormalX(double x){
        return static_cast<float>(x / WINDOW_WIDTH ) * 2.0f - 1.0f;
    }
    float calcNormalY(double x) {
        return 1.0f - static_cast<float>(x / WINDOW_HEIGHT) * 2.0f ;
    }
    double calcViewportX(float x){
        return static_cast<double>((std::abs(x) + 1.0f) / 2.0f * WINDOW_WIDTH);
    }
    double calcViewportY(float x){
        return static_cast<double>( (1.0f - std::abs(x) ) / 2.0f * WINDOW_HEIGHT);
    }

private:
    WindowParas() {}
};

namespace Renders{
extern const GLchar* vertexShaderSource;
extern const GLchar* fragmentShaderSource;
int buildVertexRender(GLFWwindow* &,GLuint&);
int buildFragementRender(GLFWwindow* &,GLuint&);
int buildShaderProgram(GLFWwindow* &,GLuint&);
}

#endif /* window_hpp */

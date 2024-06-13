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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

inline bool HAS_INIT_OPENGL_CONTEXT = false;
int initOpenGL(GLFWwindow *&window);
// build a singleton to manage all window vars
class WindowParas{
public:
    static WindowParas& getInstance(){
        static WindowParas instance;
        return instance;
    }
    WindowParas(const WindowParas&) = delete;
    void operator=(const WindowParas&) = delete;
    GLFWwindow * window;
    const GLint WINDOW_WIDTH = 1260; //960 + 300
    const GLint WINDOW_HEIGHT = 720;
    const GLint SIDEBAR_WIDTH = 300;
    GLint SCREEN_WIDTH,SCREEN_HEIGHT;
    GLfloat xScale,yScale;
    const GLfloat deltaTime = 0.016f; // 60FPS
    GLfloat defaultAlpha;
    GLfloat screen2normalX(GLdouble screenX);
    GLfloat screen2normalY(GLdouble screenY);
    GLfloat normal2orthoX(GLfloat normalX);
    GLfloat normal2orthoY(GLfloat normalY);
private:
    WindowParas() {}
};

namespace gui {
extern unsigned int panelStackNum; //count sidebar stack num,(not achieved)
extern float menuBarHeight;
void DrawGUI();
void spiltUI();
}

#endif /* window_hpp */

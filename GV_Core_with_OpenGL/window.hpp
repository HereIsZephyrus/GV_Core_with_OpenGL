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
    GLfloat screen2normalX(GLdouble screenX){
        return  (2.0f * static_cast<GLfloat>(screenX / WINDOW_WIDTH)) - 1.0f;
    }
    GLfloat screen2normalY(GLdouble screenY){
        return 1.0f - (2.0f * static_cast<GLfloat>(screenY / WINDOW_HEIGHT));
    }
    GLdouble normal2screenX(GLfloat normalX){
        return static_cast<GLdouble>((normalX + 1.0f) * WINDOW_WIDTH / 2.0f);
    }
    GLdouble normal2screenY(GLfloat normalY){
        return static_cast<GLdouble>((1.0f - normalY) * WINDOW_HEIGHT / 2.0f);
    }
private:
    WindowParas() {}
};

namespace gui {
extern bool show_demo_window;
extern bool show_another_window;
extern ImVec4 clear_color;
void DrawGUI();
}

#endif /* window_hpp */

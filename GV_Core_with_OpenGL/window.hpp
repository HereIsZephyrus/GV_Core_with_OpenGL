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
    const GLint WINDOW_WIDTH = 800;
    const GLint WINDOW_HEIGHT = 600;
    GLint SCREEN_WIDTH,SCREEN_HEIGHT;
    GLboolean keyRecord[26],pressLeft,pressRight,pressCtrl,pressShift,pressAlt;
    void initIObuffer();
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

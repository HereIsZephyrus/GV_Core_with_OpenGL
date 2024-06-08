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

private:
    WindowParas() {}
};

namespace Renders{
extern GLuint VAO;
extern GLuint VBO;
extern GLuint EBO;
}

#endif /* window_hpp */

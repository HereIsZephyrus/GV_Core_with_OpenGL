//
//  main.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 5/31/24.
//

//#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <iostream>
#include "constants.h"
#include "theme/theme_color.h"
#include "controller/commander_collection.hpp"
#include "painting/drawlines.hpp"

static int prepareWindow(GLFWwindow *&window) {
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TCB第一次图形学实验", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // to draw at this window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return  0;
}

static void bindingCallbacks(GLFWwindow *window) {
    glfwSetKeyCallback(window, keyCommand);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    return;
}

static int initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    
    return 0;
}

int main(int argc, const char * argv[]) {
    // init GLFW
    if (initOpenGL() == -1)
        return -1;
    
    // create window
    GLFWwindow * window;
    if (prepareWindow(window) == -1)
        return -2;

    // binding commands
    bindingCallbacks(window);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        drawLine();
               
        //drawLineWithMouse(window);
        glfwPollEvents();
    }

    // clean and destroy the window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

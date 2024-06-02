//
//  main.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 5/31/24.
//

#define GL_SILENCE_DEPRECATION

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "ports.h"
#include "theme/theme_color.h"
#include "painting/painting_table.hpp"
#include "controller/commander_collection.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static int prepareWindow(GLFWwindow *&window) {
    //WINDOW_WIDTH = 800;
    //WINDOW_HEIGHT = 600;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TCB第一次图形学实验", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window); // to draw at this window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return  0;
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
    
    glfwSetKeyCallback(window, keyCommand);
    
    glfwSetMouseButtonCallback(window, mouseDrawLine);
    // main loop
    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
       // glFlush();
        flushTheWindow(window);
    }

    // clean and destroy the window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

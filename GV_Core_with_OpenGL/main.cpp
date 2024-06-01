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

void drawLine() {
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
    glEnd();

    glfwSwapBuffers(glfwGetCurrentContext());
}

static int prepareWindow(GLFWwindow *&window) {
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLFW Line Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // to draw at this window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return  0;
}

static void bindingCallbacks(GLFWwindow *window) {
    glfwSetKeyCallback(window, keyCommand);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    return;
}

int main(int argc, const char * argv[]) {
    // init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // create window
    GLFWwindow * window;
    if (prepareWindow(window) == -1)
        return -2;

    // binding commands
    bindingCallbacks(window);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // 清空颜色缓冲区
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // 绘制线条
                glColor3f(1.0f, 1.0f, 1.0f);
                glBegin(GL_LINES);
                for (const auto& line : lines) {
                    glVertex2f(line.first, line.second);
                }
                glEnd();

                // 交换缓冲区
                glfwSwapBuffers(window);
                glfwPollEvents();
    }

    // clean and destroy the window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

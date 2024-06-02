//
//  drawlines.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//

#include "drawlines.hpp"

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

void drawLineWithMouse(GLFWwindow *window) {
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

//
//  commander_collection.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//
#define GL_SILENCE_DEPRECATION
#include "commander_collection.hpp"
#include "drawlines.hpp"
#include <iostream>
namespace Draw{
static bool isDrawing = false;
static double sX, sY;
}

inline float calcNormal(double x,int length){
    return (x / length) * 2.0f - 1.0f;
}
void drawing(GLFWwindow* window, double xpos, double ypos){
    float sx = calcNormal(Draw::sX, WINDOW_WIDTH);
    float sy = calcNormal(Draw::sY, WINDOW_HEIGHT);
    float tx = calcNormal(xpos, WINDOW_WIDTH);
    float ty = calcNormal(ypos, WINDOW_HEIGHT);
    drawLine(sx,sy,tx,ty);
    return;
}
void mouseDrawLine(GLFWwindow* window, int button, int action, int mods) {
    if (button != GLFW_MOUSE_BUTTON_LEFT)   return;

    if (action == GLFW_PRESS) {
        Draw::isDrawing = true;
        std::cout<<"start"<<std::endl;
        glfwGetCursorPos(window, &Draw::sX, &Draw::sY);
        glfwSetCursorPosCallback(window, drawing);
    }
    if (action == GLFW_RELEASE && Draw::isDrawing) {
        Draw::isDrawing = false;
        glfwSetCursorPosCallback(window, NULL);
        std::cout<<"end"<<std::endl;
    }
    return;
}
/*
void mouseDrawLines(GLFWwindow* window, int button, int action, int mods) {
    std::vector<fPoint> lines;
    if (button != GLFW_MOUSE_BUTTON_LEFT)   return;

    if (action == GLFW_PRESS) {
        isDrawing = true;
        glfwGetCursorPos(window, &sX, &sY);
    }
    if (action == GLFW_RELEASE && isDrawing) {
        isDrawing = false;
        glfwGetCursorPos(window, &tX, &tY);
        //drawLine();
    }
}

void mouseDrawCirle(GLFWwindow* window, int button, int action, int mods) {
    std::vector<fPoint> lines;
    if (button != GLFW_MOUSE_BUTTON_LEFT)   return;

    if (action == GLFW_PRESS) {
        isDrawing = true;
        glfwGetCursorPos(window, &sX, &sY);
    }
    if (action == GLFW_RELEASE && isDrawing) {
        isDrawing = false;
        glfwGetCursorPos(window, &tX, &tY);
        //drawLine();
    }
    return;
}
*/
void keyCommand(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

//
//  commander_collection.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//
#define GL_SILENCE_DEPRECATION
#include "commander_collection.hpp"
#include "../painting/drawlines.hpp"
#include "../window.hpp"
#include <iostream>
namespace Draw{
    static bool isDrawing = false;
    static double sX, sY,tX,tY;
}

void drawing(GLFWwindow* window, double xpos, double ypos){
    WindowParas& windowPara = WindowParas::getInstance();
    float sx = windowPara.calcNormalX(Draw::sX),  tx = windowPara.calcNormalX(xpos);
    float sy = windowPara.calcNormalY(Draw::sY),  ty = windowPara.calcNormalY(ypos);
    drawLine(sx,sy,tx,ty);
    return;
}
void mouseDrawLine(GLFWwindow* window, int button, int action, int mods) {
    WindowParas& windowPara = WindowParas::getInstance();
    if (button != GLFW_MOUSE_BUTTON_LEFT)   return;

    if (action == GLFW_PRESS) {
        Draw::isDrawing = true;
        //std::cout<<"start"<<std::endl;
        glfwGetCursorPos(window, &Draw::sX, &Draw::sY);
        table::lines.push_back(std::make_pair(windowPara.calcNormalX(Draw::sX), windowPara.calcNormalY(Draw::sY)));
        //std::cout<<table::lines.size();
        glfwSetCursorPosCallback(window, drawing);
    }
    if (action == GLFW_RELEASE && Draw::isDrawing) {
        Draw::isDrawing = false;
        glfwGetCursorPos(window, &Draw::tX, &Draw::tY);
        table::lines.push_back(std::make_pair(windowPara.calcNormalX(Draw::tX), windowPara.calcNormalY(Draw::tY)));
        //std::cout<<table::lines.size();
        glfwSetCursorPosCallback(window, NULL);
        //std::cout<<"end"<<std::endl;
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
void keyBasicCommand(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

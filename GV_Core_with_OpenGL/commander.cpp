//
//  commander_collection.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//
#define GL_SILENCE_DEPRECATION
#include "commander.hpp"
#include "window.hpp"
#include <iostream>

void MeauCommand(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)){
        if (mods & GLFW_MOD_SHIFT){
            if (key == GLFW_KEY_Z){
                //redo
            }
            if (key == GLFW_KEY_EQUAL){
                //zoom in
            }
        }
        else{
        switch (key) {
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_N:
                //new file
                break;
            case GLFW_KEY_O:
                //open file
                break;
            case GLFW_KEY_S:
                //save file
                break;
            case GLFW_KEY_W:
                //export image
                break;
            case GLFW_KEY_Z:
                //undo
                break;
            case GLFW_KEY_A:
                //add data
                break;
            case GLFW_KEY_F:
                //zoom to layer
                break;
            case GLFW_KEY_F5:
                //switch to drag
                break;
            case GLFW_KEY_F6:
                //switch to select
                break;
            case GLFW_KEY_MINUS:
                //zoom out
                break;
            default:
                break;
        }
        }
    }
}

void keyModsToggle(GLFWwindow* window, int key, int scancode, int action, int mods){
    WindowParas& windowPara = WindowParas::getInstance();
    if (action == GLFW_PRESS){
        if (mods & GLFW_MOD_CONTROL) windowPara.pressCtrl = GL_TRUE;
        if (mods & GLFW_MOD_SHIFT)  windowPara.pressShift = GL_TRUE;
        if (mods & GLFW_MOD_ALT)    windowPara.pressAlt = GL_TRUE;
    }
    if (action == GLFW_RELEASE){
        if (mods & GLFW_MOD_CONTROL) windowPara.pressCtrl = GL_FALSE;
        if (mods & GLFW_MOD_SHIFT)  windowPara.pressShift = GL_FALSE;
        if (mods & GLFW_MOD_ALT)    windowPara.pressAlt = GL_FALSE;
    }
}
void keyBasicCommand(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    MeauCommand(window, key, scancode, action, mods);
    keyModsToggle(window, key, scancode, action, mods);
    return;
}

void mouseDrawCommand(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    WindowParas& windowPara = WindowParas::getInstance();
    if (action == GLFW_PRESS){
        if (button == GLFW_MOUSE_BUTTON_LEFT)   windowPara.pressLeft = GL_TRUE;
        if (button == GLFW_MOUSE_BUTTON_RIGHT)  windowPara.pressRight = GL_TRUE;
    }
    if (action == GLFW_RELEASE){
        if (button == GLFW_MOUSE_BUTTON_LEFT)   windowPara.pressLeft = GL_FALSE;
        if (button == GLFW_MOUSE_BUTTON_RIGHT)  windowPara.pressRight = GL_FALSE;
    }
    return;
}

void mouseViewCommand(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    return;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    WindowParas& windowPara = WindowParas::getInstance();
    if (windowPara.pressCtrl){//if conflict, ctrl first
        if (yoffset > 0){
            //scroll up
        }
        if (yoffset < 0){
            //scroll down
        }
    }
    else if (windowPara.pressAlt){//if conflict, then alt
        if (yoffset > 0){
            //scroll right
        }
        if (yoffset < 0){
            //scroll left
        }
    }
    else{
        if (xoffset > 0){
            //scroll right
        }
        if (xoffset < 0){
            //scroll left
        }
        if (yoffset > 0){
            //zoom in
        }
        if (yoffset < 0){
            //zoom out
        }
    }
}

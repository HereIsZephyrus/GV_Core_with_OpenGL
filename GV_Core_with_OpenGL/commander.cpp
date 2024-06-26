//
//  commander_collection.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <memory>
#include <vector>
#include "commander.hpp"
#include "window.hpp"
#include "primitive.hpp"
#include "camera.hpp"
#include "shape.hpp"
#include "factor.hpp"

void Records::initIObuffer(){
    memset(keyRecord, GL_FALSE, sizeof(keyRecord));
    pressLeft = GL_FALSE;
    pressRight = GL_FALSE;
    pressAlt = GL_FALSE;
    pressShift = GL_FALSE;
    pressCtrl = GL_FALSE;
    dragingMode = GL_FALSE;
    drawingPrimitive = GL_FALSE;
    showAxis = GL_TRUE;
    cliping = GL_FALSE;
    draging = GL_FALSE;
    state = interectState::toselect;
    primitiveList.clear();
}

void addPoint(vertexArray& array,const GLdouble cursorX,const GLdouble cursorY){
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat normalX = windowPara.screen2normalX(cursorX);
    const GLfloat normalY = windowPara.screen2normalY(cursorY);
    const GLfloat x = windowPara.normal2orthoX(normalX);
    const GLfloat y = windowPara.normal2orthoY(normalY);
    array.push_back(x);
    array.push_back(y);
    array.push_back(0.0f); // flat draw
    //std::cout<<"add control point"<<std::endl;
}
void addPoint(vertexArray& array,const GLfloat orthoX, const GLfloat orthoY){
    array.push_back(orthoX);
    array.push_back(orthoY);
    array.push_back(0.0f); // flat draw
}
void addPoint(vertexArray& array,const GLfloat orthoX, const GLdouble cursorY){
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat normalY = windowPara.screen2normalY(cursorY);
    const GLfloat y = windowPara.normal2orthoY(normalY);
    array.push_back(orthoX);
    array.push_back(y);
    array.push_back(0.0f); // flat draw
}
void addPoint(vertexArray& array,const GLdouble cursorX, const GLfloat orthoY){
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat normalX = windowPara.screen2normalX(cursorX);
    const GLfloat x = windowPara.normal2orthoX(normalX);
    array.push_back(x);
    array.push_back(orthoY);
    array.push_back(0.0f); // flat draw
}

void keyBasicCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    MeauCallback(window, key, scancode, action, mods);
    keyModsToggle(window, key, scancode, action, mods);
    //process view move
    /*
    Records& record = Records::getState();
    double xpos,ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    const GLfloat cameraSpeed = Camera2D::getView().getCameraSpeed(400.0f);
    if (record.keyRecord[GLFW_KEY_W])
        ypos += cameraSpeed;
    if (record.keyRecord[GLFW_KEY_S])
        ypos -= cameraSpeed;
    if (record.keyRecord[GLFW_KEY_A])
        xpos += cameraSpeed;
    if (record.keyRecord[GLFW_KEY_D])
        xpos -= cameraSpeed;
    glfwSetCursorPos(window, xpos, ypos);
     */
    return;
}
void mouseDrawCallback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    mouseModsToggle(window, button, action, mods);
    drawModsToggle(window, button, action, mods);
    //tackle ondraw behavior
    Records& record = Records::getState();
    if (!Take::holdon().holdonToDraw && action == GLFW_PRESS && record.drawingPrimitive && record.pressLeft){
        GLdouble cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        addPoint(Take::holdon().drawingVertices,cursorX,cursorY);
    }
    //holdon draw method will not add middle points, which means the primitives drawed by this way has only two control points. So this situation is handled in cursorcallback
    return;
}
void mouseViewCallback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    mouseModsToggle(window, button, action, mods);
    if (Records::getState().dragingMode){
        // drag to move window
        WindowParas& windowPara = WindowParas::getInstance();
        if (button == GLFW_MOUSE_BUTTON_LEFT){
            if (action == GLFW_PRESS){
                Records::getState().draging = GL_TRUE;
                GLdouble xpos,ypos;
                Records& record = Records::getState();
                glfwGetCursorPos(window, &xpos, &ypos);
                const GLfloat cursorX =windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
                const GLfloat cursorY =windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
                record.previewXpos = cursorX;
                record.previewYpos = cursorY;
                record.previewPosition = Camera2D::getView().getPosition();
                glfwSetCursorPosCallback(window, cursorDragCallback);
            }
            else if (action == GLFW_RELEASE){
                Records::getState().draging = GL_FALSE;
                glfwSetCursorPosCallback(window, cursorSelectCallback);
            }
        }
        
    }{
        //mouse track to select primitives
    }
    return;
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    
    viewScroll(window,xoffset,yoffset);
    return;
}
void cursorDefaultCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    
    return;
}
void windowPosCallback(GLFWwindow* window, int xpos, int ypos){
    gui::spiltUI();
}
void windowSizeCallback(GLFWwindow* window, int width, int height){
    gui::spiltUI();
}

void cursorDrawCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    processCursorTrace(window,xpos,ypos);
    return;
}
void cursorDragCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    cursorDragingDetect(window,xpos,ypos);
    return;
}
void cursorSelectCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    //cursorWindowMoveDetect(window,xpos,ypos);
    return;
}
void cursorFocusCallback(GLFWwindow* window, int entered){
    ImGui_ImplGlfw_CursorEnterCallback(window, entered);
    return;
}

void MeauCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
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

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

void Records::initIObuffer(){
    memset(keyRecord, GL_FALSE, sizeof(keyRecord));
    pressLeft = GL_FALSE;
    pressRight = GL_FALSE;
    pressAlt = GL_FALSE;
    pressShift = GL_FALSE;
    pressCtrl = GL_FALSE;
    dragingMode = GL_FALSE;
    drawingPrimitive = GL_FALSE;
    state = interectState::toselect;
}

static void MeauCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void keyModsToggle(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouseModsToggle(GLFWwindow* window, int button, int action, int mods);
static void drawModsToggle(GLFWwindow* window, int button, int action, int mods);
static void viewScroll(GLFWwindow* window, double xoffset, double yoffset);

void Take::addPoint(){
    WindowParas& windowPara = WindowParas::getInstance();
    GLdouble cursorX, cursorY;
    glfwGetCursorPos(windowPara.window, &cursorX, &cursorY);
    std::cout<<"x = "<<cursorX<<','<<" y = "<<cursorY<<std::endl;
    drawingVertices.push_back(windowPara.screen2normalX(cursorX));
    drawingVertices.push_back(windowPara.screen2normalY(cursorY));
    drawingVertices.push_back(0.0f); // flat draw
}
void keyBasicCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    MeauCallback(window, key, scancode, action, mods);
    keyModsToggle(window, key, scancode, action, mods);
    return;
}
void mouseDrawCallback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    mouseModsToggle(window, button, action, mods);
    drawModsToggle(window, button, action, mods);
    //tackle ondraw behavior
    Records& record = Records::getState();
    if (!Take::holdon().holdonToDraw && action == GLFW_PRESS && record.drawingPrimitive && record.pressLeft)
            Take::holdon().addPoint();
    //holdon draw method will not add middle points, which means the primitives drawed by this way has only two control points. So this situation is handled in cursorcallback
    return;
}
void mouseViewCallback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    mouseModsToggle(window, button, action, mods);
    return;
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    
    viewScroll(window,xoffset,yoffset);
    return;
}
void cursorSelectCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    
    return;
}
void cursorDrawCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    if (Records::getState().drawingPrimitive){//generate preview
        //take the last point
        vertexArray& vertices = Take::holdon().drawingVertices;
        vertexArray::const_reverse_iterator it = vertices.rbegin();
        WindowParas& windowPara = WindowParas::getInstance();
        vertexArray tempVertices;
        tempVertices.push_back(*(it+2));    //last x
        tempVertices.push_back(*(it+1));    //last y
        tempVertices.push_back(*(it));        //last z
        tempVertices.push_back(windowPara.screen2normalX(xpos));    //cursor x
        tempVertices.push_back(windowPara.screen2normalY(ypos));    //cursor y
        tempVertices.push_back(0.0f);        //cursor z
        //generate preview primitive
        pPrimitive previewPrimitive(new Primitive(tempVertices,GL_LINES,3));
        previewPrimitive -> bindShader(rd::shaders["singleWhite"].get());
        pr::drawPreviewPrimitive = std::move(previewPrimitive);
    }
    else
        pr::drawPreviewPrimitive = nullptr;
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
void keyModsToggle(GLFWwindow* window, int key, int scancode, int action, int mods){
    Records& record = Records::getState();
    if (action == GLFW_PRESS){
        record.keyRecord[key] = GL_TRUE;
        if (mods & GLFW_MOD_CONTROL) record.pressCtrl = GL_TRUE;
        if (mods & GLFW_MOD_SHIFT)  record.pressShift = GL_TRUE;
        if (mods & GLFW_MOD_ALT)    record.pressAlt = GL_TRUE;
        
    }
    if (action == GLFW_RELEASE){
        record.keyRecord[key] = GL_FALSE;
        if (mods & GLFW_MOD_CONTROL) record.pressCtrl = GL_FALSE;
        if (mods & GLFW_MOD_SHIFT)  record.pressShift = GL_FALSE;
        if (mods & GLFW_MOD_ALT)    record.pressAlt = GL_FALSE;
    }
}
void mouseModsToggle(GLFWwindow* window, int button, int action, int mods){
    Records& record = Records::getState();
    if (action == GLFW_PRESS){
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            record.pressLeft = GL_TRUE;
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
            record.pressRight = GL_TRUE;
    }
    if (action == GLFW_RELEASE){
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            record.pressLeft = GL_FALSE;
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
            record.pressRight = GL_FALSE;
    }
}
void viewScroll(GLFWwindow* window, double xoffset, double yoffset){
    Camera2D* camera = static_cast<Camera2D*>(glfwGetWindowUserPointer(window));
    Records& record = Records::getState();
    if (record.pressCtrl){//if conflict, ctrl first
        if (yoffset > 0){
            //scroll up
        }
        if (yoffset < 0){
            //scroll down
        }
    }
    else if (record.pressAlt){//if conflict, then alt
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
        camera->zoomInOut(static_cast<float>(yoffset));
    }
}
static bool startDrawCheck(GLFWwindow* window, int button, int action, int mods){
    Records& record = Records::getState();
    if (action != GLFW_PRESS || !record.pressLeft) return false; // check left click
    if ( record.state != interectState::drawing || Take::holdon().drawType == GL_POINT) return false; //check ready to start
    return !record.drawingPrimitive; //check aleady started
}
static bool finishDrawCheck(GLFWwindow* window, int button, int action, int mods){
    Records& record = Records::getState();
    if (!record.drawingPrimitive) return false; //check under drawing
    if (action == GLFW_PRESS){//click right to stop
        if (button == GLFW_MOUSE_BUTTON_RIGHT && !Take::holdon().holdonToDraw)
            return true;
        return false;
    }
    else if (action == GLFW_RELEASE){//release left to stop
        if (button == GLFW_MOUSE_BUTTON_LEFT && Take::holdon().holdonToDraw)
            return true;
        return false;
    }
    return false;
}
void drawModsToggle(GLFWwindow* window, int button, int action, int mods){
    Records& record = Records::getState();
    Take& take = Take::holdon();
    if (startDrawCheck(window, button, action, mods)){
        record.drawingPrimitive = true;
        //generate a new primitive
        std::cout<<"start draw"<<std::endl;
        take.drawingVertices.clear();
        take.addPoint();
    }
    if (finishDrawCheck(window,button,action,mods)){
        record.drawingPrimitive = false;
        //finish the draw and push into the formal primitive render queue
        take.addPoint();
        std::cout<<"finish draw"<<std::endl;
        pr::createPrimitiveTo(pr::primitives);
        take.drawType = GL_POINT;
    }
}

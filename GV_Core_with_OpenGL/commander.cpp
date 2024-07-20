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
#include "objectmodel.hpp"

void Records::initIObuffer(){
    memset(keyRecord, GL_FALSE, sizeof(keyRecord));
    pressLeft = GL_FALSE;
    pressRight = GL_FALSE;
    pressAlt = GL_FALSE;
    pressShift = GL_FALSE;
    pressCtrl = GL_FALSE;
    doubleCliked = GL_FALSE;
    dragingMode = GL_FALSE;
    drawingPrimitive = GL_FALSE;
    showAxis = true;
    editingString = false;
    cliping = GL_FALSE;
    draging = GL_FALSE;
    state = interectState::toselect;
    primitiveList.clear();
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
    doubleClickDetected(window, button, action, mods);
    //tackle ondraw behavior
    GLdouble cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    if (Take::holdon().drawType == Shape::MARKER && WindowParas::getInstance().mainWindowFocused){
        WindowParas& windowPara = WindowParas::getInstance();
        const GLfloat pointSize = ShaderStyle::getStyle().pointsize;
        const glm::vec3 position = glm::vec3(
                                             windowPara.normal2orthoX(windowPara.screen2normalX(cursorX))/pointSize,
                                             windowPara.normal2orthoY(windowPara.screen2normalY(cursorY))/pointSize,
                                             0.0f);
        ObjectModel newLogo = obj::markers["logo"];
        newLogo.setScale(pointSize);
        newLogo.setPosition(position);
        obj::objectList.push_back(newLogo);
        return;
    }
    Records& record = Records::getState();
    if (!Take::holdon().holdonToDraw && action == GLFW_PRESS && record.drawingPrimitive && record.pressLeft)
        addPoint(Take::holdon().drawingVertices,cursorX,cursorY);
    //holdon draw method will not add middle points, which means the primitives drawed by this way has only two control points. So this situation is handled in cursorcallback
    return;
}
void mouseViewCallback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    mouseModsToggle(window, button, action, mods);
    doubleClickDetected(window, button, action, mods);
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
void mouseEditCallback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    mouseModsToggle(window, button, action, mods);
    doubleClickDetected(window, button, action, mods);
    WindowParas& windowPara = WindowParas::getInstance();
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        Records& record = Records::getState();
        Take& take = Take::holdon();
        GLdouble xpos,ypos;
        glfwGetCursorPos(windowPara.window, &xpos, &ypos);
        const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
        const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
        record.previewXpos = cursorX;
        record.previewYpos = cursorY;
        if (windowPara.mainWindowFocused)
            take.editingPrimitive->outBound->relationship = take.editingPrimitive->outBound->cursorDetect(xpos, ypos);
    }
    if (action == GLFW_RELEASE && windowPara.mainWindowFocused){
        Take& take = Take::holdon();
        take.editingPrimitive->exertTransmat(take.editingPrimitive->outBound->getTransmat());
        take.editingPrimitive->outBound->setTransmat(take.editingPrimitive->getTransMat());
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

static bool checkCursorFocus(){
    WindowParas& windowPara = WindowParas::getInstance();
    GLdouble cursorX,cursorY;
    glfwGetCursorPos(windowPara.window, &cursorX, &cursorY);
    //std::cout<<cursorX<<' '<<cursorY<<std::endl;
    if (cursorX < 0 || cursorX > windowPara.SCREEN_WIDTH/windowPara.xScale || cursorY< gui::menuBarHeight || cursorY > windowPara.SCREEN_HEIGHT/windowPara.yScale)
        return  false;
    //bool openDetect = ((Records::getState().state == interectState::holding) || (Records::getState().state == interectState::toselect));
    if (!Records::getState().dragingMode){
        Camera2D& camera = Camera2D::getView();
        const GLfloat dragCameraSpeed = gui::dragCameraSpeed * camera.getZoom();
        if (cursorX < gui::borderDetectRange){
            camera.setDeltaPosition(camera.getPosition(), -dragCameraSpeed, 0);
        }
        else if (cursorX> windowPara.SCREEN_WIDTH/windowPara.xScale - gui::borderDetectRange){
            camera.setDeltaPosition(camera.getPosition(), dragCameraSpeed, 0);
        }
        if (cursorY < gui::borderDetectRange + gui::menuBarHeight && cursorX > gui::menuWidth){
            camera.setDeltaPosition(camera.getPosition(), 0, dragCameraSpeed);
        }
        else if (cursorY> windowPara.SCREEN_HEIGHT/windowPara.yScale - gui::borderDetectRange){
            camera.setDeltaPosition(camera.getPosition(), 0, -dragCameraSpeed);
        }
    }
    return true;
}

int InterectResponseCheck(GLFWwindow* &window){
    Camera2D& camera = Camera2D::getView();
    camera.processKeyboard(window);
    coord::generateCoordinateAxis();
    WindowParas::getInstance().mainWindowFocused = checkCursorFocus();
    if (Records::getState().state == interectState::editing && WindowParas::getInstance().mainWindowFocused)
        outboundInterectCheck(Take::holdon().editingPrimitive->outBound);
    return 0;
}

int releaseResources(GLFWwindow* &window){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    WindowParas::getInstance().window = nullptr;
    Records::getState().primitiveList.clear();
    return 0;
}

bool primitiveSelectDetect(Primitive* primitive){
    GLdouble xpos,ypos;
    WindowParas& windowPara = WindowParas::getInstance();
    glfwGetCursorPos(windowPara.window, &xpos, &ypos);
    bool selected = false;
    if (primitive->getShape() == GL_POINTS){
        for (auto singlePoint = primitive ->elementList.begin(); singlePoint != primitive->elementList.end(); singlePoint ++){
            selected = (*singlePoint)->cursorSelectDetect(xpos, ypos);
            if (selected)   break;
        }
    }else{
        pElement characterPrimitive = primitive -> elementList.back();
        selected = (*characterPrimitive).cursorSelectDetect(xpos, ypos);
    }
    return selected;
}

void keyModsToggle(GLFWwindow* window, int key, int scancode, int action, int mods){
    Records& record = Records::getState();
    if (record.editingString)
        return;
    if (action == GLFW_PRESS){
        //std::cout<<"press "<<key<<std::endl;
        record.keyRecord[key] = GL_TRUE;
        if (record.keyRecord[GLFW_KEY_LEFT_CONTROL] || record.keyRecord[GLFW_KEY_RIGHT_CONTROL]) record.pressCtrl = GL_TRUE;
        if (record.keyRecord[GLFW_KEY_LEFT_SHIFT] || record.keyRecord[GLFW_KEY_RIGHT_SHIFT])  record.pressShift = GL_TRUE;
        if (record.keyRecord[GLFW_KEY_LEFT_ALT] || record.keyRecord[GLFW_KEY_RIGHT_ALT])    record.pressAlt = GL_TRUE;
        
    }
    if (action == GLFW_RELEASE){
        //std::cout<<"release "<<key<<std::endl;
        record.keyRecord[key] = GL_FALSE;
        if (!record.keyRecord[GLFW_KEY_LEFT_CONTROL] && !record.keyRecord[GLFW_KEY_RIGHT_CONTROL]) record.pressCtrl = GL_FALSE;
        if (!record.keyRecord[GLFW_KEY_LEFT_SHIFT] && !record.keyRecord[GLFW_KEY_RIGHT_SHIFT])  record.pressShift = GL_FALSE;
        if (!record.keyRecord[GLFW_KEY_LEFT_ALT] && !record.keyRecord[GLFW_KEY_RIGHT_ALT])    record.pressAlt = GL_FALSE;
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
    Camera2D::getView().processScroll(window, xoffset, yoffset, Records::getState().pressCtrl, Records::getState().pressAlt);
    coord::generateCoordinateAxis();
}
static bool startDrawCheck(GLFWwindow* window, int button, int action, int mods){
    Records& record = Records::getState();
    if (!WindowParas::getInstance().mainWindowFocused)  return false; // the start points must in the window range
    if (action != GLFW_PRESS || !record.pressLeft) return false; // check left click
    if ( record.state != interectState::drawing || Take::holdon().drawType == Shape::NONE ) return false; //check ready to start
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
    Take& take = Take::holdon();
    if (startDrawCheck(window, button, action, mods)){
        Records::getState().drawingPrimitive = true;
        std::cout<<"start draw"<<std::endl;
        take.drawingVertices.clear();
        //generate the first point to the new primitive
        if (take.holdonToDraw){
            GLdouble cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            addPoint(take.drawingVertices,cursorX,cursorY);
        }
    }
    if (finishDrawCheck(window,button,action,mods)){
        Records& record = Records::getState();
        record.drawingPrimitive = false;
        //finish the primitive
        if (take.holdonToDraw){
            GLdouble cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            addPoint(take.drawingVertices,cursorX,cursorY);
            if (record.pressCtrl)
                toAlignment(take.drawingVertices,take.drawType);
        }
        std::cout<<"finish draw"<<std::endl;
        // push the primitive into the formal primitive render queue
        generateNewPrimitive();
        take.drawType = Shape::NONE;
    }
}
void cursorDragingDetect(GLFWwindow* window,double xpos, double ypos){
    Records& record = Records::getState();
    WindowParas& windowPara = WindowParas::getInstance();
    if (record.draging){
        const GLfloat cursorX =windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
        const GLfloat cursorY =windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
        const GLfloat dX = (record.previewXpos - cursorX);
        const GLfloat dY = (record.previewYpos - cursorY);
        Camera2D::getView().setDeltaPosition(record.previewPosition,dX,dY);
    }
}

void processCursorTrace(GLFWwindow* window,double xpos, double ypos){
    Take& take = Take::holdon();
    if (Records::getState().drawingPrimitive && take.drawType != Shape::MARKER){//generate preview
        //take the last point
        vertexArray tempVertices;
        tempVertices = take.drawingVertices;
        addPoint(tempVertices,xpos,ypos);
        if (Records::getState().pressCtrl)
            toAlignment(tempVertices,take.drawType);
        generatePreviewPrimitive(tempVertices);
    }
    else
        pr::previewPrimitive = nullptr;
    return;
}
void doubleClickDetected(GLFWwindow* window, int button, int action, int mods){
    GLdouble& lastClickTime = WindowParas::getInstance().lastClickTime;
    GLboolean& doubleClickState = Records::getState().doubleCliked;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            GLdouble currentTime = glfwGetTime();
        if (currentTime - lastClickTime < gui::doubleClickBias)
                doubleClickState = GL_TRUE;
            else
                doubleClickState = GL_FALSE;
            lastClickTime = currentTime;
        }
}

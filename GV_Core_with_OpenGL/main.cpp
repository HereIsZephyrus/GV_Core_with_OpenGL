//
//  main.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 5/31/24.
//

//#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <filesystem>
#include <memory>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.hpp"
#include "rendering.hpp"
#include "primitive.hpp"
#include "commander.hpp"
#include "camera.hpp"
#include "shape.hpp"
#include "factor.hpp"
#include "objectmodel.hpp"
#include "initialization.hpp"

static int releaseResources(GLFWwindow* &window);
static int InterectResponseCheck(GLFWwindow* &window);

int main(int argc, const char * argv[]) {
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window) != 0)
        return -1;
    initImGUI(window);
    initInterect(window);
    initStyle();
    initObject();
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        gui::DrawGUI();
        ImVec4 backgroundColor = WindowParas::getInstance().backgroundColor;
        glClearColor(backgroundColor.x,backgroundColor.y, backgroundColor.z, backgroundColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        InterectResponseCheck(window);
        bool hasHolding = false;
        Records& record = Records::getState();
        //draw main primitive list
        
        bool openDetect = ((record.state == interectState::holding) || (record.state == interectState::toselect)); // whether primitives can be select
        for (auto primitive = pr::mainPrimitiveList.begin(); primitive!= pr::mainPrimitiveList.end(); primitive++){
            // detect hold
            if (WindowParas::getInstance().mainWindowFocused && openDetect && !record.dragingMode && record.pressLeft)
                 primitiveSelectDetect((*primitive).get());
            hasHolding |= (*primitive)->getHold();
            // draw elements
            (*primitive)->useShader();
            for (auto element = (*primitive)->elementList.begin(); element!=(*primitive)->elementList.end(); element++)
                (*element)->draw((*primitive)->getHold());
        }
        if (openDetect && !hasHolding && record.pressLeft &&  !record.pressCtrl){
            Take::holdon().holdonObjList.clear();
            record.state = interectState::toselect;
        }
        
        // draw priview
        if (pr::previewPrimitive != nullptr){
            pr::previewPrimitive -> draw();
            //std::cout<<"showing preview"<<std::endl;
        }
        // draw objects
        //obj::markers["logo"].draw();
        for (auto object = obj::objectList.begin(); object!= obj::objectList.end(); object++)
            (*object).draw();
        //draw axis
        if (Records::getState().showAxis && pr::axisPrimitive != nullptr){
            pr::axisPrimitive -> draw();
            coord::drawScaleText();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    
    releaseResources(window);
    return 0;
}

static bool checkCursorFocus(){
    WindowParas& windowPara = WindowParas::getInstance();
    GLdouble cursorX,cursorY;
    glfwGetCursorPos(windowPara.window, &cursorX, &cursorY);
    //std::cout<<cursorX<<' '<<cursorY<<std::endl;
    if (cursorX < 0 || cursorX > windowPara.SCREEN_WIDTH/windowPara.xScale || cursorY< gui::menuBarHeight || cursorY > windowPara.SCREEN_HEIGHT/windowPara.yScale)
        return  false;
    bool openDetect = ((Records::getState().state == interectState::holding) || (Records::getState().state == interectState::toselect));
    if (openDetect && !Records::getState().dragingMode){
        Camera2D& camera = Camera2D::getView();
        const GLfloat dragCameraSpeed = 10.0f,borderDetectRange = 40.0f, menuWidth = 200.0f;
        if (cursorX < borderDetectRange){
            camera.setDeltaPosition(camera.getPosition(), -dragCameraSpeed, 0);
        }
        else if (cursorX> windowPara.SCREEN_WIDTH/windowPara.xScale - borderDetectRange){
            camera.setDeltaPosition(camera.getPosition(), dragCameraSpeed, 0);
        }
        if (cursorY < borderDetectRange  && cursorX > menuWidth){
            camera.setDeltaPosition(camera.getPosition(), 0, dragCameraSpeed);
        }
        else if (cursorY> windowPara.SCREEN_HEIGHT/windowPara.yScale - borderDetectRange){
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
        editPrimitive();
    return 0;
}

int releaseResources(GLFWwindow* &window){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    WindowParas::getInstance().window = nullptr;
    Take::holdon().editingPrimitive = nullptr;
    Take::holdon().drawingShader = nullptr;
    Records::getState().primitiveList.clear();
    return 0;
}

bool primitiveSelectDetect(Primitive* primitive){
    GLdouble xpos,ypos;
    WindowParas& windowPara = WindowParas::getInstance();
    Records& record = Records::getState();
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
    
    if (selected){
        //std::cout<<"selected"<<std::endl;
        record.state = interectState::holding;
        if (primitive->getHold())
            return selected;
        else{
            if (!record.pressCtrl)
                Take::holdon().holdonObjList.clear();
            Take::holdon().holdonObjList.push_back(primitive);
        }
        primitive->setHold(selected);
        //std::cout<<"selected"<<Take::holdon().holdonObjList.size()<<std::endl;
    }
    else{
       // std::cout<<"not selected"<<Take::holdon().holdonObjList.size()<<std::endl;
        if (!record.pressCtrl)
            primitive->setHold(selected);
    }
    return selected;
}

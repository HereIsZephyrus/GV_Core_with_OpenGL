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
            //std::cout<<(*primitive)->layer<<std::endl;
            if ((*primitive)->visable){
                (*primitive)->useShader();
                for (auto element = (*primitive)->elementList.begin(); element!=(*primitive)->elementList.end(); element++)
                    (*element)->draw((*primitive)->getHold());
            }
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

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
//#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.hpp"
#include "rendering.hpp"
#include "primitive.hpp"
#include "commander.hpp"
#include "camera.hpp"

static int initImGUI(GLFWwindow *window);
static int initInterect(GLFWwindow* &window);
static int initStyle();
static int releaseResources(GLFWwindow* &window);
static int InterectResponseCheck(GLFWwindow* &window);
int main(int argc, const char * argv[]) {
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window) != 0)
        return -1;
    initImGUI(window);
    initInterect(window);
    initStyle();
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        gui::DrawGUI();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        InterectResponseCheck(window);
        //std::cout<<WindowParas::getInstance().mainWindowFocused<<std::endl;
        for (auto it = pr::mainPrimitiveList.begin(); it!= pr::mainPrimitiveList.end(); it++)
            (*it)->draw();
        if (pr::drawPreviewPrimitive != nullptr){
            pr::drawPreviewPrimitive -> draw();
            //std::cout<<"existed"<<std::endl;
        }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    
    releaseResources(window);
    return 0;
}

int initImGUI(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    gui::spiltUI();
    ShaderStyle& style = ShaderStyle::getStyle();
    style.drawColor =  ImVec4(1.0f, 0.5f, 0.2f, 1.0f);
    style.thickness = 1.0f;
    Take& take = Take::holdon();
    take.drawType = Shape::NONE; //set as blank style
    gui::menuBarHeight = ImGui::GetFrameHeightWithSpacing() * WindowParas::getInstance().yScale;
    return  0;
}

int initInterect(GLFWwindow* &window){
    Records::getState().initIObuffer();
    glfwSetWindowPosCallback(window, windowPosCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetKeyCallback(window, keyBasicCallback);
    glfwSetMouseButtonCallback(window, mouseViewCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, cursorSelectCallback);
    glfwSetCursorEnterCallback(window, cursorFocusCallback);
    return 0;
}
static void checkSourceRelevantPath(){
    namespace fs = std::filesystem;
    fs::path cwd = fs::current_path();
    fs::path otherPath = "/Users/channingtong/Program/GV_Core_with_OpenGL/resources";
    fs::path relativePath = relative(otherPath, cwd);
    std::cout << "Current directory: " << cwd << std::endl;
    std::cout << "Other path: " << otherPath << std::endl;
    std::cout << "Relative path: " << relativePath << std::endl;
}

int initStyle(){
    //init shader
    pShader singleYellow (new Shader(rd::filePath("singleVertices.vs"),rd::filePath("fillYellow.frag")));
    rd::namedShader["singleYellow"] = std::move(singleYellow);
    pShader singleWhite (new Shader(rd::filePath("singleVertices.vs"),rd::filePath("fillWhite.frag")));
    rd::namedShader["singleWhite"] = std::move(singleWhite);
    rd::defaultShader = rd::namedShader["singleYellow"].get();
    
    //init camera
    WindowParas& windowPara = WindowParas::getInstance();
    pCamera2D tempZeroCamera = pCamera2D(new CameraPara2D(glm::vec2(windowPara.SIDEBAR_WIDTH/2, 0.0f),1.0f,WindowParas::getInstance().SCREEN_WIDTH,WindowParas::getInstance().SCREEN_HEIGHT));
    cm::zeroCamera = std::move(tempZeroCamera);
    Camera2D::getView().loadSavedPara(cm::zeroCamera.get());
    //init primitive paras
    Records& record = Records::getState();
    record.pointSize = 5.0f;
    glPointSize(record.pointSize);
    return 0;
}

static bool checkCursorFocus(){
    WindowParas& windowPara = WindowParas::getInstance();
    GLdouble cursorX,cursorY;
    glfwGetCursorPos(windowPara.window, &cursorX, &cursorY);
    //std::cout<<cursorX<<' '<<cursorY<<std::endl;
    if (cursorX >= 0 && cursorX <= windowPara.SCREEN_WIDTH/windowPara.xScale && cursorY>= 0 && cursorY <= windowPara.SCREEN_HEIGHT/windowPara.yScale)
        return  true;
    return false;
}

int InterectResponseCheck(GLFWwindow* &window){
    Camera2D& camera = Camera2D::getView();
    camera.processKeyboard(window);
    WindowParas::getInstance().mainWindowFocused = checkCursorFocus();
    return 0;
}

int releaseResources(GLFWwindow* &window){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

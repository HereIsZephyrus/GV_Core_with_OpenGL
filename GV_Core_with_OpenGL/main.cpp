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
//    this is a demo
    pr::triangle-> bindShader(rd::shaders["singleYellow"].get());
    pr::rectangle->bindShader(rd::shaders["singleWhite"].get());
    pr::primitives.push_back(std::move(pr::triangle));
    pr::primitives.push_back(std::move(pr::rectangle));
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        gui::DrawGUI();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        InterectResponseCheck(window);
        for (auto it = pr::primitives.begin(); it!= pr::primitives.end(); it++)
            (*it)->draw();
        if (pr::drawPreviewPrimitive != nullptr)
            pr::drawPreviewPrimitive -> draw();
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
    
    ShaderStyle& style = ShaderStyle::getStyle();
    style.drawColor =  ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.thickness = 1.0f;
    Take& take = Take::holdon();
    take.drawType = GL_POINT; //set as blank style
    return  0;
}

int initInterect(GLFWwindow* &window){
    Records::getState().initIObuffer();
    glfwSetKeyCallback(window, keyBasicCallback);
    glfwSetMouseButtonCallback(window, mouseViewCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, cursorSelectCallback);
    glfwSetCursorEnterCallback(window, cursorFocusCallback);
    return 0;
}
static GLchar* filePath(const char* fileName){
    namespace fs = std::filesystem;
    fs::path cwd = fs::current_path();

    // 检查其他路径对当前路径的相对路径
    fs::path otherPath = "/Users/channingtong/Program/GV_Core_with_OpenGL/resources";
    fs::path relativePath = relative(otherPath, cwd);

    //std::cout << "Current directory: " << cwd << std::endl;
    //std::cout << "Other path: " << otherPath << std::endl;
    //std::cout << "Relative path: " << relativePath << std::endl;
    const char * searchPath ="../../../../../../../../Program/GV_Core_with_OpenGL/resources/";
    GLchar* resource = new char[strlen(searchPath) + strlen(fileName) + 1];
    strcpy(resource, searchPath);
    strcat(resource, fileName);
    //std::cout<<resource<<std::endl;
    return resource;
}
int initStyle(){
    //init shader
    pShader singleYellow (new Shader(rd::singleVertices, rd::fillYellow));
    //pShader singleYellow (new Shader(filePath("singleVertices.vs"),filePath("fillYellow.frag")));
    rd::shaders["singleYellow"] = std::move(singleYellow);
    pShader singleWhithe (new Shader(rd::singleVertices, rd::fillWhite));
    rd::shaders["singleWhite"] = std::move(singleWhithe);
    rd::defaultShader = rd::shaders["singleYellow"].get();
    
    //init camera
    return 0;
}

int InterectResponseCheck(GLFWwindow* &window){
    Camera2D::getView().processKeyboard(window, WindowParas::getInstance().deltaTime);
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

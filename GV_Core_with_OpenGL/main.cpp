//
//  main.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 5/31/24.
//

//#define GL_SILENCE_DEPRECATION
#include <iostream>
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
int main(int argc, const char * argv[]) {
    GLFWwindow *& window = WindowParas::getInstance().window;
    if (!HAS_INIT_OPENGL_CONTEXT && initOpenGL(window) != 0)
        return -1;
    initImGUI(window);
    initInterect(window);
    initStyle();
    
    
    Camera2D camera(800, 600);
    glfwSetWindowUserPointer(window, &camera);
//    this is a demo
    pr::triangle-> bindShader(rd::defaultShader);
    pr::rectangle->bindShader(rd::defaultShader);
    pr::primitives.push_back(std::move(pr::triangle));
    pr::primitives.push_back(std::move(pr::rectangle));
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        gui::DrawGUI();
        float deltaTime = 0.016f; // 假设每帧约 60FPS

        camera.processKeyboard(window, deltaTime);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        Shader* shaderProgram = rd::shaders["singleYellow"].get();
        glm::mat4 projection = camera.getProjectionMatrix();
       glm::mat4 view = camera.getViewMatrix();
       glm::mat4 model = glm::mat4(1.0f);

       GLuint projectionLoc = glGetUniformLocation(shaderProgram->getProgram(), "projection");
       GLuint viewLoc = glGetUniformLocation(shaderProgram->getProgram(), "view");
       GLuint modelLoc = glGetUniformLocation(shaderProgram->getProgram(), "model");

       glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
       glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
       glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
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
int initStyle(){
    //init shader
    pShader singleYellow (new Shader(rd::singleVertices, rd::fillYellow));
    rd::shaders["singleYellow"] = std::move(singleYellow);
    pShader singleWhithe (new Shader(rd::singleVertices, rd::fillWhite));
    rd::shaders["singleWhite"] = std::move(singleWhithe);
    rd::defaultShader = rd::shaders["singleYellow"].get();
    
    //init camera
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

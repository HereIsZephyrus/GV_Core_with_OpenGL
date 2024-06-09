//
//  main.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 5/31/24.
//

//#define GL_SILENCE_DEPRECATION
#include <iostream>
//#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <SDL2/SDL_opengl.h>
#include "window.hpp"
#include "rendering.hpp"
#include "primitive.hpp"
#include "commander.hpp"
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

static int initOpenGL(GLFWwindow *&window) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    //SDL_Init(SDL_INIT_EVERYTHING);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    //sf::ContextSettings settings;
    //settings.depthBits = 24;
    //settings.stencilBits = 8;
    //settings.majorVersion = 4;
    //settings.minorVersion = 1;
    //settings.attributeFlags = sf::ContextSettings::Core;
    
    WindowParas& windowPara = WindowParas::getInstance();
    window = glfwCreateWindow(windowPara.WINDOW_WIDTH, windowPara.WINDOW_HEIGHT, "TCBOpenGL学习", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    return 0;
}
/*
static int initImGUI(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    return  0;
}
*/

static int initVIew(GLFWwindow* &window){
    WindowParas& windowPara = WindowParas::getInstance();
    glfwGetFramebufferSize(window, &windowPara.SCREEN_WIDTH, &windowPara.SCREEN_HEIGHT);
    glfwMakeContextCurrent(window); // to draw at this window
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()){
        std::cerr << "Failed to initialize GLEW"<<std::endl;
        return -2;
    }
    glViewport(0, 0, windowPara.SCREEN_WIDTH, windowPara.SCREEN_HEIGHT);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout<<version<<std::endl;
    return 0;
}

void bindBasicCommand(GLFWwindow* &window){
    glfwSetKeyCallback(window, keyBasicCommand);
    //glfwSetMouseButtonCallback(window, mouseDrawLine);
    return;
}
int main(int argc, const char * argv[]) {
    // init GLEW and GLFW
    GLFWwindow * window;
    if (initOpenGL(window) != 0)
        return -1;
    if (initVIew(window) != 0 )
        return -1;
    //initImGUI(window);
    
    Shader singleYellow = Shader(rd::singleVertices, rd::fillYellow);
    Primitive triangle = Primitive(pr::tranVertex, GL_TRIANGLES, 3, 3);
    Primitive rectangle = Primitive(pr::rectVertex, pr::indices, GL_TRIANGLES, 4, 3, 6);
   
    bindBasicCommand(window);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    // main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        singleYellow.Rend();
        triangle.draw();
        rectangle.draw();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

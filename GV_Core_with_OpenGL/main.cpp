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
#include "controller/commander_collection.hpp"
#include "rendering.hpp"
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
    window = glfwCreateWindow(windowPara.WINDOW_WIDTH, windowPara.WINDOW_HEIGHT, "TCB第一次图形学实验", nullptr, nullptr);
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
void destoryResources(GLFWwindow* &window){
    glDeleteVertexArrays(1,&Renders::VAO);
    glDeleteBuffers(1,&Renders::VBO);
    glDeleteBuffers(1,&Renders::EBO);
    // clean and destroy the window
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
}
int main(int argc, const char * argv[]) {
    // init GLEW and GLFW
    GLFWwindow * window;
    if (initOpenGL(window) != 0)
        return -1;
    
    //initImGUI(window);
    
    if (initVIew(window) != 0 )
        return -1;
    
    Shader singleYellow = Shader(rd::singleVertices, rd::fillYellow);
    
    GLfloat vertices[] =
    {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
         -0.5f, 0.5f, 0.0f
    };
    GLuint indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    //Renders::bindPermitive(Renders::VAO,Renders::VBO,vertices);
    glGenVertexArrays(1,&Renders::VAO);
    glBindVertexArray(Renders::VAO);
    
    glGenBuffers(1,&Renders::VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Renders::VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &Renders::EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renders::EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renders::EBO);
    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    bindBasicCommand(window);
    
    // main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        singleYellow.Rend();
        glBindVertexArray(Renders::VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    
    destoryResources(window);
    return 0;
}

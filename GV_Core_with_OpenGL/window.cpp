//
//  window.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/8/24.
//
#include <iostream>
#include "commander.hpp"
#include "window.hpp"
#include "camera.hpp"

int initOpenGL(GLFWwindow *&window) {
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
    glfwGetWindowContentScale(window, &windowPara.xScale, &windowPara.yScale);
    std::cout<<"This Screen xScale is"<<windowPara.xScale<<",yScale is"<<windowPara.yScale<<std::endl;
    glfwGetFramebufferSize(window, &windowPara.SCREEN_WIDTH, &windowPara.SCREEN_HEIGHT);
    windowPara.SCREEN_WIDTH = windowPara.SCREEN_WIDTH - windowPara.SIDEBAR_WIDTH * windowPara.xScale;
    glfwMakeContextCurrent(window); // to draw at this window
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()){
        std::cerr << "Failed to initialize GLEW"<<std::endl;
        return -2;
    }
    glViewport(windowPara.SIDEBAR_WIDTH * windowPara.xScale, 0, windowPara.SCREEN_WIDTH, windowPara.SCREEN_HEIGHT);
    
    glfwSetWindowUserPointer(window, &Camera2D::getView());
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout<<version<<std::endl;
    HAS_INIT_OPENGL_CONTEXT = true;
    return 0;
}

namespace gui{
static void renderMenu(GLFWwindow *&window);
static void renderEditPanel();
static void renderSiderbar();
static void renderSelectPanel();
void DrawGUI() {
    GLFWwindow *& window = WindowParas::getInstance().window;
    Records& record = Records::getState();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    renderMenu(window);
    renderSiderbar();
    if (record.state == interectState::drawing) renderEditPanel();
    if (record.showCreateElementWindow)             renderSelectPanel();
    ImGui::Render();
    return;
}
static void projectMenu() {
    if (ImGui::MenuItem("New", "CTRL N")){
        
    }
    if (ImGui::MenuItem("Open", "CTRL O")){
        
    }
    if (ImGui::MenuItem("Save", "CTRL S")){
        
    }
    if (ImGui::MenuItem("Export", "CTRL W"))
    {
        
    }
}

static void editMenu() {
    Records& record = Records::getState();
    if (ImGui::MenuItem("Undo", "CTRL Z")){
        
    }
    if (ImGui::MenuItem("Redo", "CTRL Shift Z")){
        
    }
    if (record.state == interectState::toselect && ImGui::MenuItem("Start edit")){
        record.state = interectState::drawing;
        glfwSetMouseButtonCallback(WindowParas::getInstance().window, mouseDrawCallback);
    }
    
    if (record.state == interectState::drawing && ImGui::MenuItem("Stop edit")){
        record.state = interectState::toselect;
        glfwSetMouseButtonCallback(WindowParas::getInstance().window, mouseViewCallback);
    }
    
    if (ImGui::MenuItem("Add Data", "CTRL A")){
        
    }
}

static void viewMenu() {
    if (ImGui::MenuItem("Zoom to layer", "CTRL F")){
        
    }
    if (ImGui::MenuItem("Zoom in", "CTRL Alt +")){
        
    }
    if (ImGui::MenuItem("Zoom out", "CTRL Alt -")){
        
    }
    if (ImGui::MenuItem("Drag", "F5"))
        Records::getState().dragingMode = GL_TRUE;
    if (ImGui::MenuItem("Select", "F6"))
        Records::getState().dragingMode = GL_FALSE;
}

void renderMenu(GLFWwindow *&window) {
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("MyGIS")){
            if (ImGui::MenuItem("About")){
                // 处理新建操作
            }
            if (ImGui::MenuItem("Quit", "Ctrl Q")){
                //warning for save
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Project")){
            projectMenu();
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit")){
            editMenu();
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")){
            viewMenu();
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void renderSiderbar(){
    WindowParas& windowPara = WindowParas::getInstance();
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowPara.SIDEBAR_WIDTH, windowPara.SCREEN_HEIGHT), ImGuiCond_Always);
    ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Text("This is the main sidebar");
    ImGui::End();
}
void renderEditPanel(){
    Records& record = Records::getState();
    ShaderStyle& style = ShaderStyle::getStyle();
    ImGui::Begin("Edit Panel");
    ImGui::Text("This is a edit panel");
    if (ImGui::Button("Create Element"))
        record.showCreateElementWindow = true;
    ImGui::ColorEdit4("Color", (float*)&style.drawColor);
    ImGui::SliderFloat("Thickness", &style.thickness, 1.0f, 10.0f);
    ImGui::Checkbox("Fill", &style.toFill);
    ImGui::End();
}

void renderSelectPanel(){
    Records& record = Records::getState();
    GLenum& drawType = Take::holdon().drawType;
    bool& holdonToDraw = Take::holdon().holdonToDraw;
    ImGui::Begin("Create Element", &record.showCreateElementWindow, ImGuiWindowFlags_AlwaysAutoResize);
    
    if (ImGui::Button("Points")){
        drawType = GL_POINTS;
        holdonToDraw = false;
        record.showCreateElementWindow = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Lines")){
        drawType = GL_LINES;
        holdonToDraw = true;
        record.showCreateElementWindow = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Strip")){
        drawType = GL_LINE_STRIP;
        holdonToDraw = false;
        record.showCreateElementWindow = false;
    }
    if (ImGui::Button("Polygen")){
        if (ShaderStyle::getStyle().toFill)
            drawType = GL_TRIANGLE_FAN;
        else
            drawType = GL_LINE_LOOP;
        holdonToDraw = false;
        record.showCreateElementWindow = false;
    }
    if (ImGui::Button("Trangles")){
        drawType = GL_TRIANGLES;
        holdonToDraw = false;
        record.showCreateElementWindow = false;
    }
    if (ImGui::Button("Rectangle")){
        drawType = GL_TRIANGLE_STRIP;
        holdonToDraw = true;
        record.showCreateElementWindow = false;
    }
    if (ImGui::Button("Circle")){
        drawType = GL_TRIANGLE_FAN;
        holdonToDraw = true;
        record.showCreateElementWindow = false;
    }
    
    ImGui::End();
}
}//namespace gui

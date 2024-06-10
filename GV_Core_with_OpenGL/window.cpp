//
//  window.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/8/24.
//
#include <iostream>
#include "commander.hpp"
#include "window.hpp"

namespace gui{
static void renderMenu(GLFWwindow *&window);
static void renderEditPanel();
void DrawGUI() {
    GLFWwindow *& window = WindowParas::getInstance().window;
    Records& record = Records::getState();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    renderMenu(window);
    if (record.state == interectState::drawing) renderEditPanel();
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

static void renderMenu(GLFWwindow *&window) {
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
static void renderEditPanel(){
    ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
       float thickness = 1.0f;
       bool fill = false;
       int drawType = 0; // 0: 点, 1: 线, 2: 面
    ImGui::Begin("Edit Panel");
        ImGui::Text("This is a edit panel");
        ImGui::RadioButton("Point", &drawType, 0); ImGui::SameLine();
        ImGui::RadioButton("Line", &drawType, 1); ImGui::SameLine();
        ImGui::RadioButton("Fill", &drawType, 2);

        // 设置颜色
        ImGui::ColorEdit4("Color", (float*)&color);

        // 设置线条粗细
        ImGui::SliderFloat("Thickness", &thickness, 1.0f, 10.0f);

        // 是否填充
        ImGui::Checkbox("Fill", &fill);
    ImGui::End();
}
}

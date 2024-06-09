//
//  window.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/8/24.
//
#include <iostream>
#include "window.hpp"

namespace gui{
void showDemo(){
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;
        
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        
        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);
        
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
        
        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
    
    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
    return;
}

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static void projectMenu() {
    if (ImGui::MenuItem("New", "CTRL N"))
    {
        
    }
    if (ImGui::MenuItem("Open", "CTRL O"))
    {
        
    }
    if (ImGui::MenuItem("Save", "CTRL S"))
    {
        
    }
    if (ImGui::MenuItem("Export", "CTRL W"))
    {
        
    }
}

static void editMenu() {
    if (ImGui::MenuItem("Undo", "CTRL Z"))
    {
        
    }
    if (ImGui::MenuItem("Redo", "CTRL Shift Z"))
    {
        
    }
    if (ImGui::MenuItem("Edit Attributes"))
    {
        
    }
    if (ImGui::MenuItem("Edit geometry"))
    {
        
    }
    if (ImGui::MenuItem("Add Data", "CTRL A"))
    {
        
    }
}

static void viewMenu() {
    if (ImGui::MenuItem("Zoom to layer", "CTRL F"))
    {
        
    }
    if (ImGui::MenuItem("Zoom in", "CTRL Alt +"))
    {
        
    }
    if (ImGui::MenuItem("Zoom out", "CTRL Alt -"))
    {
        
    }
    if (ImGui::MenuItem("Drag", "F5"))
    {
        
    }
    if (ImGui::MenuItem("Select", "F6"))
    {
        
    }
}

void DrawGUI() {
    GLFWwindow *& window = WindowParas::getInstance().window;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // 在主渲染循环中
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
    //showDemo();
    ImGui::Render();
    return;
}
}

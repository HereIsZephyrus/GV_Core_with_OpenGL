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
#include "primitive.hpp"

GLfloat WindowParas::screen2normalX(GLdouble screenX){
    return  (2.0f * static_cast<GLfloat>(screenX/ SCREEN_WIDTH * xScale)) - 1.0f;
}
GLfloat WindowParas::screen2normalY(GLdouble screenY){
    return 1.0f - (2.0f * static_cast<GLfloat>(screenY / SCREEN_HEIGHT * yScale));
}
GLfloat WindowParas::normal2orthoX(GLfloat normalX){
    const float zoom = Camera2D::getView().getZoom();
    GLfloat left = -SCREEN_WIDTH / xScale / 2.0f * zoom;
    GLfloat right = SCREEN_WIDTH / xScale / 2.0f * zoom;
    return  left + (right - left) * (normalX + 1) / 2 + Camera2D::getView().getPosition().x;
}
GLfloat WindowParas::normal2orthoY(GLfloat normalY){
    const float zoom = Camera2D::getView().getZoom();
    GLfloat button = -SCREEN_HEIGHT / yScale / 2.0f * zoom;
    GLfloat top = SCREEN_HEIGHT / yScale / 2.0f * zoom;
    return  button + (top - button) * (normalY + 1) / 2 + Camera2D::getView().getPosition().y;
}
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
    window = glfwCreateWindow(windowPara.WINDOW_WIDTH, windowPara.WINDOW_HEIGHT, "2024Spring计算机图形学", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwGetWindowContentScale(window, &windowPara.xScale, &windowPara.yScale);
    std::cout<<"This Screen xScale is"<<windowPara.xScale<<",yScale is"<<windowPara.yScale<<std::endl;
    glfwMakeContextCurrent(window); // to draw at this window
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()){
        std::cerr << "Failed to initialize GLEW"<<std::endl;
        return -2;
    }
    glfwSetWindowUserPointer(window, &Camera2D::getView());
    windowPara.defaultAlpha = glfwGetWindowOpacity(window);
    
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout<<version<<std::endl;
    HAS_INIT_OPENGL_CONTEXT = true;
    return 0;
}

namespace gui {
unsigned int panelStackNum = 0;
float menuBarHeight;
}

namespace gui{
static void renderMenu(GLFWwindow *&window);
static void renderEditPanel();
static void renderSiderbar();
static void renderSelectPanel();
void DrawGUI() {
    GLFWwindow *& window = WindowParas::getInstance().window;
    Records& record = Records::getState();
    if (glfwGetWindowOpacity(window) != WindowParas::getInstance().defaultAlpha)
        spiltUI();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->ChannelsSplit(3);
    drawList->ChannelsSetCurrent(0);
    renderSiderbar();
    drawList->ChannelsSetCurrent(1);
    renderMenu(window);
    drawList->ChannelsSetCurrent(2);
    if (record.state == interectState::drawing) renderEditPanel();
    if (record.showCreateElementWindow)         renderSelectPanel();
    drawList->ChannelsMerge();
    ImGui::Render();
    return;
}
void spiltUI(){
    WindowParas& windowPara = WindowParas::getInstance();
    glfwGetFramebufferSize(windowPara.window, &windowPara.SCREEN_WIDTH, &windowPara.SCREEN_HEIGHT);
    windowPara.SCREEN_WIDTH = windowPara.SCREEN_WIDTH - windowPara.SIDEBAR_WIDTH * windowPara.xScale;
    glViewport(0, 0, windowPara.SCREEN_WIDTH, windowPara.SCREEN_HEIGHT);
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
        panelStackNum ++;
        GLFWwindow *& window = WindowParas::getInstance().window;
        glfwSetMouseButtonCallback(window, mouseDrawCallback);
        glfwSetCursorPosCallback(window, cursorDrawCallback);
    }
    
    if (record.state == interectState::drawing && ImGui::MenuItem("Stop edit")){
        record.state = interectState::toselect;
        panelStackNum --;
        GLFWwindow *& window = WindowParas::getInstance().window;
        glfwSetMouseButtonCallback(window, mouseViewCallback);
        glfwSetCursorPosCallback(window, cursorSelectCallback);
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
    ImGui::SetNextWindowPos(ImVec2(windowPara.SCREEN_WIDTH/windowPara.xScale, menuBarHeight), ImGuiCond_Always);
    float sidebarHeight = (windowPara.SCREEN_HEIGHT / 2)*(1 + (panelStackNum==0))/windowPara.yScale;
    ImGui::SetNextWindowSize(ImVec2(windowPara.SIDEBAR_WIDTH,sidebarHeight - menuBarHeight), ImGuiCond_Always);
    ImGui::Begin("sidebar",nullptr,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Text("This is the main sidebar");
    ImGui::End();
}
void renderEditPanel(){
    Records& record = Records::getState();
    ShaderStyle& style = ShaderStyle::getStyle();
    WindowParas& windowPara = WindowParas::getInstance();
    ImGui::SetNextWindowPos(ImVec2(windowPara.SCREEN_WIDTH/windowPara.xScale, windowPara.SCREEN_HEIGHT/windowPara.yScale/2), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowPara.SIDEBAR_WIDTH, windowPara.SCREEN_HEIGHT/windowPara.yScale/2), ImGuiCond_Always);
    ImGui::Begin("Edit Panel",nullptr, ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize);
    ImGui::Text("This is a edit panel");
    if (ImGui::Button("Create Element"))
        record.showCreateElementWindow = true;
    ImGui::ColorEdit4("Color", (float*)&style.drawColor);
    ImGui::SliderFloat("Thickness", &style.thickness, 1.0f, 10.0f);
    ImGui::Checkbox("Fill", &style.toFill);
    Shape& drawType = Take::holdon().drawType;
    bool& holdonToDraw = Take::holdon().holdonToDraw;
    if (!record.cliping && ImGui::Button("Clip")){
        record.cliping = true;
    }
    if (record.cliping && ImGui::Button("Finish Clip")){
        record.cliping = false;
    }
    if (record.cliping){
        if (ImGui::Button("Use Rectangle")){
            drawType = Shape::LOOP;
            holdonToDraw = true;
        }
        if (ImGui::Button("Use Polygon")){
            drawType = Shape::POLYGEN;
            holdonToDraw = false;
        }
        //std::cout<<(drawType == Shape::RECTANGLE)<<std::endl;
    }
    ImGui::End();
}

void renderSelectPanel(){
    Records& record = Records::getState();
    Shape& drawType = Take::holdon().drawType;
    bool& holdonToDraw = Take::holdon().holdonToDraw;
    WindowParas& windowPara = WindowParas::getInstance();
    ImGui::SetNextWindowPos(ImVec2(windowPara.SCREEN_WIDTH/windowPara.xScale, windowPara.SCREEN_HEIGHT/windowPara.yScale/2), ImGuiCond_Always);
    ImGui::Begin("Create Element", &record.showCreateElementWindow, ImGuiWindowFlags_AlwaysAutoResize);
    
    if (ImGui::Button("Points")){
        drawType = Shape::POINTS;
        holdonToDraw = false;
        record.showCreateElementWindow = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Lines")){
        drawType = Shape::LINES;
        holdonToDraw = true;
        record.showCreateElementWindow = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Strip")){
        drawType = Shape::STRIPE;
        holdonToDraw = false;
        record.showCreateElementWindow = false;
    }
    if (ImGui::Button("Polygon")){
        if (ShaderStyle::getStyle().toFill)
            drawType = Shape::POLYGEN;
        else
            drawType = Shape::LOOP;
        holdonToDraw = false;
        record.showCreateElementWindow = false;
    }
    if (ImGui::Button("Trangles")){
        drawType = Shape::TRIANGLE;
        holdonToDraw = false;
        record.showCreateElementWindow = false;
    }
    if (ImGui::Button("Rectangle")){
        drawType = Shape::RECTANGLE;
        holdonToDraw = true;
        record.showCreateElementWindow = false;
    }
    if (ImGui::Button("Circle")){
        drawType = Shape::CIRCLE;
        holdonToDraw = true;
        record.showCreateElementWindow = false;
    }
    
    ImGui::End();
}
}//namespace gui

void ClipByShape(){
    //Records& record = Records::getState();
    Take& take = Take::holdon();
    //ShaderStyle& style = ShaderStyle::getStyle();
    for (auto it = pr::mainPrimitiveList.begin(); it!= pr::mainPrimitiveList.end(); it++){
        vertexArray& vertices = (*it) ->vertices;
        std::cout<<"before"<<vertices.size()<<std::endl;
        const GLsizei stride = (*it)->stride;
        vertexArray newArray;
        newArray.clear();
        if ((*it)->shape == GL_POINTS && take.drawType == Shape::LOOP){
            const GLfloat x1 = take.drawingVertices[0],x2 = take.drawingVertices[6];
            const GLfloat y1 = take.drawingVertices[1],y2 = take.drawingVertices[7];
            const GLfloat xMax = std::max(x1,x2),xMin = std::min(x1,x2);
            const GLfloat yMax = std::max(y1,y2),yMin = std::min(y1,y2);
            //auto ind = vertices.begin();
            for (auto ind = vertices.begin(); ind!=vertices.end(); ind+=stride){
                if ((*(ind) >= xMin) && (*(ind) <= xMax) && (*(ind+1) >= yMin) && (*(ind+1) <= yMax)){
                    newArray.insert(newArray.end(), ind,ind+stride);
                }
            }
            vertices = newArray;
            std::cout<<"after"<<vertices.size()<<std::endl;
            (*it)->updateVertex();
        }
    }
}

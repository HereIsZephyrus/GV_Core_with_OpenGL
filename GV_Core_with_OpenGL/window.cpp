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
#include "shape.hpp"

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
    //ImGui::Text("This is the main sidebar");
    createPrimitiveList();
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
void createPrimitiveList() {
    std::vector<item >& items = Records::getState().primitiveList;
    for (int i = 0; i < items.size(); i++) {
        int currentIndex = i;
        items[i].first->layer = i+1;
        bool isHovered = ImGui::IsItemHovered();
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload("TEXT_LIST_ITEM", &currentIndex, sizeof(int));
            if (isHovered) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
            }
            ImGui::Text("%s", items[currentIndex].second.c_str());
            if (isHovered) {
                ImGui::PopStyleColor(2);
            }
            ImGui::EndDragDropSource();
        }
        if (isHovered) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
        }
        ImGui::TextUnformatted(items[currentIndex].second.c_str());
        if (isHovered) {
            ImGui::PopStyleColor(2);
        }
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXT_LIST_ITEM")) {
                int sourceIndex = *(const int*)payload->Data;
                std::swap(items[currentIndex], items[sourceIndex]);
            }
            ImGui::EndDragDropTarget();
            std::sort(pr::mainPrimitiveList.begin(),pr::mainPrimitiveList.end());
        }

        ImGui::Separator();
    }
}

}//namespace gui

void clipPoints(vertexArray& vertices, const GLsizei& stride,const GLfloat& xMin,const GLfloat& xMax,const GLfloat& yMin,const GLfloat& yMax){
    vertexArray newArray;
    newArray.clear();
    //std::cout<<"before"<<vertices.size()<<std::endl;
    
    for (auto ind = vertices.begin(); ind!=vertices.end(); ind+=stride)
        if ((*(ind) >= xMin) && (*(ind) <= xMax) && (*(ind+1) >= yMin) && (*(ind+1) <= yMax))
            newArray.insert(newArray.end(), ind,ind+stride);
    vertices = newArray;
    //std::cout<<"after"<<vertices.size()<<std::endl;
}

static int getRegionCode(const GLfloat& x, const GLfloat& y,const GLfloat& xMin,const GLfloat& xMax,const GLfloat& yMin,const GLfloat& yMax) {
    int code = 0;
    if (x < xMin) code |= left_bit_code; //left
    if (x > xMax) code |= right_bit_code; //right
    if (y < yMin) code |= button_bit_code; //button
    if (y > yMax) code |= top_bit_code; //top
    return code;
}
void clipCohenSutherLand(vertexArray& vertices, const GLsizei& stride,const GLfloat& xMin,const GLfloat& xMax,const GLfloat& yMin,const GLfloat& yMax){
    vertexArray newArray;
    newArray.clear();
    //std::cout<<"before"<<vertices.size()<<std::endl;
    for (auto ind = vertices.begin(); ind!=vertices.end(); ind+=stride*2){
        const GLfloat ox1 = *(ind),oy1 = *(ind+1),ox2 = *(ind+stride),oy2 = *(ind+stride+1);
        int code1 = getRegionCode(ox1, oy1, xMin, xMax, yMin, yMax);
        int code2 = getRegionCode(ox2, oy2, xMin, xMax, yMin, yMax);
        
        if ((code1 & code2) != 0) //completely outside
            continue;
        
        if (code1 == 0 && code2 == 0) //completely inside
            newArray.insert(newArray.end(), ind,ind + stride*2);
        
        GLfloat x=0 , y=0;
        GLfloat x1 = ox1, x2 = ox2, y1 = oy1, y2 = oy2;
        while (true) {
            if (code1 != 0) {
                if ((code1 & top_bit_code) != 0) {
                    x = x1 + (x2 - x1) * (yMax - y1) / (y2 - y1);
                    y = yMax;
                } else if ((code1 & button_bit_code) != 0) {
                    x = x1 + (x2 - x1) * (yMin - y1) / (y2 - y1);
                    y = yMin;
                } else if ((code1 & right_bit_code) != 0) {
                    x = xMax;
                    y = y1 + (y2 - y1) * (xMax - x1) / (x2 - x1);
                } else if ((code1 & left_bit_code) != 0) {
                    x = xMin;
                    y = y1 + (y2 - y1) * (xMin - x1) / (x2 - x1);
                }
                x1 = x; y1 = y;
                code1 = getRegionCode(x1, y1, xMin, xMax, yMin, yMax);
            } else {
                if ((code2 & top_bit_code) != 0) {
                    x = x1 + (x2 - x1) * (yMax - y1) / (y2 - y1);
                    y = yMax;
                } else if ((code2 & button_bit_code) != 0) {
                    x = x1 + (x2 - x1) * (yMin - y1) / (y2 - y1);
                    y = yMin;
                } else if ((code2 & right_bit_code) != 0) {
                    x = xMax;
                    y = y1 + (y2 - y1) * (xMax - x1) / (x2 - x1);
                } else if ((code2 & left_bit_code) != 0) {
                    x = xMin;
                    y = y1 + (y2 - y1) * (xMin - x1) / (x2 - x1);
                }
                x2 = x; y2 = y;
                code2 = getRegionCode(x2, y2, xMin, xMax, yMin, yMax);
            }
            
            if (code1 == 0 && code2 == 0) {//completely inside
                newArray.push_back(x1);
                newArray.push_back(y1);
                newArray.push_back(0.0f);
                newArray.push_back(x2);
                newArray.push_back(y2);
                newArray.push_back(0.0f);
                break;
            } else if ((code1 & code2) != 0) {//completely outside
                break;
            }
        }
    }
    vertices = newArray;
    //std::cout<<"after"<<vertices.size()<<std::endl;
}
GLfloat x_intersect(const GLfloat& x1, const GLfloat& y1, const GLfloat& x2, const GLfloat& y2,const GLfloat& x3, const GLfloat& y3, const GLfloat& x4, const GLfloat& y4){
    const GLfloat& num = (x1*y2 - y1*x2) * (x3-x4) -
              (x1-x2) * (x3*y4 - y3*x4);
    const GLfloat& den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}
GLfloat y_intersect(const GLfloat& x1, const GLfloat& y1, const GLfloat& x2, const GLfloat& y2,const GLfloat& x3, const GLfloat& y3, const GLfloat& x4, const GLfloat& y4){
    const GLfloat& num = (x1*y2 - y1*x2) * (y3-y4) -
              (y1-y2) * (x3*y4 - y3*x4);
    const GLfloat& den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}

void clipSutherlandHodgman(vertexArray& vertices, const GLsizei& stride,const GLfloat& xMin,const GLfloat& xMax,const GLfloat& yMin,const GLfloat& yMax){
    vertexArray newArray;
    newArray.clear();
    std::cout<<"before"<<vertices.size()<<std::endl;
    for (auto ind = vertices.begin(); ind!=vertices.end(); ind+=stride){
        GLfloat x1 = *(ind),y1 = *(ind+1),x2,y2;
        if (ind+1 ==vertices.end()){
            x2 = *(vertices.begin());
            y2 = *(vertices.begin() + 1);
        }
        else{
            x2 = *(ind + stride);
            y2 = *(ind + stride + 1);
        }
        bool inside1 = (x1 >= xMin && x1 <= xMax && y1 >= yMin && y1 <= yMax);
        bool inside2 = (x2 >= xMin && x2 <= xMax && y2 >= yMin && y2 <= yMax);
        //if (!inside1 && !inside2)   continue; //both outside
        if (inside1 && inside2) {//both inside
            newArray.push_back(x1);
            newArray.push_back(y1);
            newArray.push_back(0.0f);
        } else if(inside1 && !inside2){
            GLfloat intersectX = x_intersect(xMin,yMin,xMax,yMax,x1, y1, x2, y2);
            GLfloat intersectY = y_intersect(xMin,yMin,xMax,yMax,x1, y1, x2, y2);
            newArray.push_back(intersectX);
            newArray.push_back(intersectY);
            newArray.push_back(0.0f);
            newArray.push_back(x2);
            newArray.push_back(y2);
            newArray.push_back(0.0f);
        } else if (!inside1 && inside2) {
            GLfloat intersectX = x_intersect(xMin,yMin,xMax,yMax,x1, y1, x2, y2);
            GLfloat intersectY = y_intersect(xMin,yMin,xMax,yMax,x1, y1, x2, y2);
            newArray.push_back(intersectX);
            newArray.push_back(intersectY);
            newArray.push_back(0.0f);
            }
    }
    vertices = newArray;
    std::cout<<"after"<<vertices.size()<<std::endl;
}
void clipLiangBarsky(vertexArray& vertices, const GLsizei& stride,const GLfloat& xMin,const GLfloat& xMax,const GLfloat& yMin,const GLfloat& yMax){
    vertexArray newArray;
    newArray.clear();
    for (auto ind = vertices.begin(); ind!=vertices.end(); ind+=stride*2){
        const GLfloat x1 = *(ind),y1 = *(ind+1),x2 = *(ind+stride),y2 = *(ind+stride+1);
        GLfloat p[4], q[4];
        p[0] = -(x2 - x1);  p[1] = x2 - x1;     p[2] = -(y2 - y1);  p[3] = y2 - y1;
        q[0] = x1 - xMin;   q[1] = xMax - x1;   q[2] = y1 - yMin;   q[3] = yMax - y1;
        GLfloat u1 = 0.0f, u2 = 1.0f;
        for (int i = 0; i < 4; i++) {
            if (p[i] == 0.0f) {
                if (q[i] < 0.0f)    continue;
            } else {
                GLfloat t = q[i] / p[i];
                if (p[i] < 0.0f) {
                    if (t > u2) continue;
                    if (t > u1) u1 = t;
                } else {
                    if (t < u1) continue;
                    if (t < u2) u2 = t;
                }
            }
        }
        const GLfloat nx1 = x1 + u1 * (x2 - x1),ny1 = y1 + u1 * (y2 - y1);
        const GLfloat nx2 = x1 + u2 * (x2 - x1),ny2 = y1 + u2 * (y2 - y1);
        newArray.push_back(nx1);
        newArray.push_back(ny1);
        newArray.push_back(0.0f);
        newArray.push_back(nx2);
        newArray.push_back(ny2);
        newArray.push_back(0.0f);
    }
    vertices = newArray;
}
void clipLiangBarsky(vertexArray& vertices, const GLsizei& stride){
    vertexArray newArray;
    newArray.clear();
    const vertexArray& clipVertex = Take::holdon().drawingVertices;
    for (auto ind = vertices.begin(); ind!=vertices.end(); ind+=stride*2){
        GLfloat p[4], q[4];
        const GLfloat x1 = *(ind),y1 = *(ind+1),x2 = *(ind+stride),y2 = *(ind+stride+1);
        GLfloat u1 = 0.0f, u2 = 1.0f;
        for (auto clipInd1 = clipVertex.begin(); clipInd1 != clipVertex.end(); clipInd1 +=3){
            vertexArray::const_iterator clipInd2;
            if (clipInd1 + 3 == clipVertex.end())
                clipInd2 = clipVertex.begin();
            else
                clipInd2 = clipInd1 + 3;
            const GLfloat vx1 = *(clipInd1),vy1 = *(clipInd1+1),vx2 = *(clipInd2),vy2 = *(clipInd2+1);
            p[0] = -vx2 + vx1;  p[1] = x1 - x2;     p[2] = -vy2 + vy1;  p[3] = y1 - y2;
            q[0] = x2 - vx1;    q[1] = y2 - vy1;    q[2] = x2 - vx2;    q[3] = y2 - vy2;
            for (int k = 0; k < 4; k++) {
                if (p[k] == 0.0f) {
                    if (q[k] < 0.0f) {
                        break;
                    }
                } else {
                    float t = q[k] / p[k];
                    if (p[k] < 0.0f) {
                        if (t > u2) break;
                        if (t > u1) u1 = t;
                    } else {
                        if (t < u1) break;
                        if (t < u2) u2 = t;
                    }
                }
            }
        }
        const GLfloat nx1 = x1 + u1 * (x2 - x1),ny1 = y1 + u1 * (y2 - y1);
        const GLfloat nx2 = x1 + u2 * (x2 - x1),ny2 = y1 + u2 * (y2 - y1);
        newArray.push_back(nx1);
        newArray.push_back(ny1);
        newArray.push_back(0.0f);
        newArray.push_back(nx2);
        newArray.push_back(ny2);
        newArray.push_back(0.0f);
    }
    vertices = newArray;
}
void clipByShape(){
    Take& take = Take::holdon();
    for (auto it = pr::mainPrimitiveList.begin(); it!= pr::mainPrimitiveList.end(); it++){
        vertexArray& vertices = (*it) ->vertices;
        const GLsizei stride = (*it)->stride;
        if (take.drawType == Shape::LOOP){
            const GLfloat x1 = take.drawingVertices[0],x2 = take.drawingVertices[6];
            const GLfloat y1 = take.drawingVertices[1],y2 = take.drawingVertices[7];
            const GLfloat xMax = std::max(x1,x2),xMin = std::min(x1,x2);
            const GLfloat yMax = std::max(y1,y2),yMin = std::min(y1,y2);
            if ((*it)->shape == GL_POINTS){
                clipPoints(vertices,stride,xMin,xMax,yMin,yMax);
                (*it)->updateVertex();
                pr::updateIndex((*it).get());
            }
            if ((*it)->shape == GL_LINES){
                //clipCohenSutherLand(vertices, stride, xMin, xMax, yMin, yMax);
                clipLiangBarsky(vertices, stride, xMin, xMax, yMin, yMax);
                (*it)->updateVertex();
                pr::updateIndex((*it).get());
            }
            if ((*it)->shape == GL_LINE_LOOP || (*it)->shape == GL_TRIANGLE_FAN ){
                //clipSutherlandHodgman(vertices, stride, xMin, xMax, yMin, yMax);
                //(*it)->updateVertex();
            }
        }
        else{
            //clipLiangBarsky(vertices, stride);
            //(*it)->updateVertex();
        }
    }
}


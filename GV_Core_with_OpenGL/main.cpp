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
#include "shape.hpp"
#include "factor.hpp"

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
        ImVec4 backgroundColor = WindowParas::getInstance().backgroundColor;
        glClearColor(backgroundColor.x,backgroundColor.y, backgroundColor.z, backgroundColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        InterectResponseCheck(window);
        bool hasHolding = false;
        Records& record = Records::getState();
        //draw main primitive list
        
        bool openDetect = ((record.state == interectState::holding) || (record.state == interectState::toselect)); // whether primitives can be select
        for (auto primitive = pr::mainPrimitiveList.begin(); primitive!= pr::mainPrimitiveList.end(); primitive++){
            // detect hold
            if (WindowParas::getInstance().mainWindowFocused && openDetect && !record.dragingMode && record.pressLeft)
                 primitiveSelectDetect((*primitive).get());
            hasHolding |= (*primitive)->getHold();
            // draw elements
            (*primitive)->useShader();
            for (auto element = (*primitive)->elementList.begin(); element!=(*primitive)->elementList.end(); element++)
                (*element)->draw((*primitive)->getHold());
        }
        if (openDetect && !hasHolding && record.pressLeft &&  !record.pressCtrl){
            Take::holdon().holdonObjList.clear();
            record.state = interectState::toselect;
        }
        
        //draw priview
        if (pr::drawPreviewPrimitive != nullptr){
            pr::drawPreviewPrimitive -> draw();
            //std::cout<<"showing preview"<<std::endl;
        }
        
        //draw axis
        if (Records::getState().showAxis && pr::axisPrimitive != nullptr){
            pr::axisPrimitive -> draw();
            coord::drawScaleText();
        }
        ImGui::Render();
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
    style.thickness = 5.0f;
    style.pointsize = 5.0f;
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

static void checkStyleBoundary() {
    GLfloat lineWidthRange[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidthRange);
    std::cout << "Supported line width range: " << lineWidthRange[0] << " to " << lineWidthRange[1] << std::endl;
    GLfloat minPointSize, maxPointSize;
    glGetFloatv(GL_POINT_SIZE_RANGE, &minPointSize);
    glGetFloatv(GL_POINT_SIZE_MAX, &maxPointSize);
    std::cout << "Supported point size range: " << minPointSize << " to " << maxPointSize<< std::endl;
}

int initStyle(){
    
    //init camera
    WindowParas& windowPara = WindowParas::getInstance();
    pCamera2D tempZeroCamera = pCamera2D(new CameraPara2D(glm::vec2(0.0f, 0.0f),1.0f,WindowParas::getInstance().SCREEN_WIDTH,WindowParas::getInstance().SCREEN_HEIGHT));
    cm::zeroCamera = std::move(tempZeroCamera);
    Camera2D::getView().loadSavedPara(cm::zeroCamera.get());
    //init primitive paras
    glEnable(GL_LINE_SMOOTH);
    checkStyleBoundary();
    windowPara.backgroundColor = {0.1f, 0.1f, 0.1f, 1.0f};
    
    //init preview shader
    pShader previewfillShader (new Shader());
    previewfillShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
    previewfillShader->attchShader(rd::filePath("fillWhite.frag"),GL_FRAGMENT_SHADER);
    previewfillShader->linkProgram();
    rd::namedShader["previewfillShader"] = std::move(previewfillShader);
    pShader previewlineShader (new Shader());
    previewlineShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
    previewlineShader->attchShader(rd::filePath("fillWhite.frag"),GL_FRAGMENT_SHADER);
    previewlineShader->attchShader(rd::filePath("lineWidth.gs"), GL_GEOMETRY_SHADER);
    previewlineShader->linkProgram();
    rd::namedShader["previewlineShader"] = std::move(previewlineShader);
    //init axis
    pShader axisShader (new Shader());
    axisShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
    axisShader->attchShader(rd::filePath("fillWhite.frag"),GL_FRAGMENT_SHADER);
    axisShader->linkProgram();
    rd::namedShader["axisShader"] = std::move(axisShader);
    coord::generateCoordinateAxis();
    return 0;
}

static bool checkCursorFocus(){
    WindowParas& windowPara = WindowParas::getInstance();
    GLdouble cursorX,cursorY;
    glfwGetCursorPos(windowPara.window, &cursorX, &cursorY);
    //std::cout<<cursorX<<' '<<cursorY<<std::endl;
    if (cursorX < 0 || cursorX > windowPara.SCREEN_WIDTH/windowPara.xScale || cursorY< gui::menuBarHeight || cursorY > windowPara.SCREEN_HEIGHT/windowPara.yScale)
        return  false;
    bool openDetect = ((Records::getState().state == interectState::holding) || (Records::getState().state == interectState::toselect));
    if (openDetect && !Records::getState().dragingMode){
        Camera2D& camera = Camera2D::getView();
        const GLfloat dragCameraSpeed = 10.0f,borderDetectRange = 40.0f, menuWidth = 200.0f;
        if (cursorX < borderDetectRange){
            camera.setDeltaPosition(camera.getPosition(), -dragCameraSpeed, 0);
        }
        else if (cursorX> windowPara.SCREEN_WIDTH/windowPara.xScale - borderDetectRange){
            camera.setDeltaPosition(camera.getPosition(), dragCameraSpeed, 0);
        }
        if (cursorY < borderDetectRange  && cursorX > menuWidth){
            camera.setDeltaPosition(camera.getPosition(), 0, dragCameraSpeed);
        }
        else if (cursorY> windowPara.SCREEN_HEIGHT/windowPara.yScale - borderDetectRange){
            camera.setDeltaPosition(camera.getPosition(), 0, -dragCameraSpeed);
        }
    }
    return true;
}

int InterectResponseCheck(GLFWwindow* &window){
    Camera2D& camera = Camera2D::getView();
    camera.processKeyboard(window);
    coord::generateCoordinateAxis();
    WindowParas::getInstance().mainWindowFocused = checkCursorFocus();
    if (Records::getState().state == interectState::editing && WindowParas::getInstance().mainWindowFocused)
        editPrimitive();
    return 0;
}

int releaseResources(GLFWwindow* &window){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    WindowParas::getInstance().window = nullptr;
    Take::holdon().editingPrimitive = nullptr;
    Take::holdon().drawingShader = nullptr;
    Records::getState().primitiveList.clear();
    return 0;
}

bool primitiveSelectDetect(Primitive* primitive){
    GLdouble xpos,ypos;
    WindowParas& windowPara = WindowParas::getInstance();
    Records& record = Records::getState();
    glfwGetCursorPos(windowPara.window, &xpos, &ypos);
    bool selected = false;
    if (primitive->getShape() == GL_POINTS){
        for (auto singlePoint = primitive ->elementList.begin(); singlePoint != primitive->elementList.end(); singlePoint ++){
            selected = (*singlePoint)->cursorSelectDetect(xpos, ypos);
            if (selected)   break;
        }
    }else{
        pElement characterPrimitive = primitive -> elementList.back();
        selected = (*characterPrimitive).cursorSelectDetect(xpos, ypos);
    }
    
    if (selected){
        //std::cout<<"selected"<<std::endl;
        record.state = interectState::holding;
        if (primitive->getHold())
            return selected;
        else{
            if (!record.pressCtrl)
                Take::holdon().holdonObjList.clear();
            Take::holdon().holdonObjList.push_back(primitive);
        }
        primitive->setHold(selected);
        //std::cout<<"selected"<<Take::holdon().holdonObjList.size()<<std::endl;
    }
    else{
       // std::cout<<"not selected"<<Take::holdon().holdonObjList.size()<<std::endl;
        if (!record.pressCtrl)
            primitive->setHold(selected);
    }
    return selected;
}

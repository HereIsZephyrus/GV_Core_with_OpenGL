//
//  commander_collection.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <memory>
#include <vector>
#include "commander.hpp"
#include "window.hpp"
#include "primitive.hpp"
#include "camera.hpp"

void Records::initIObuffer(){
    memset(keyRecord, GL_FALSE, sizeof(keyRecord));
    pressLeft = GL_FALSE;
    pressRight = GL_FALSE;
    pressAlt = GL_FALSE;
    pressShift = GL_FALSE;
    pressCtrl = GL_FALSE;
    dragingMode = GL_FALSE;
    drawingPrimitive = GL_FALSE;
    state = interectState::toselect;
}

static void MeauCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void keyModsToggle(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouseModsToggle(GLFWwindow* window, int button, int action, int mods);
static void drawModsToggle(GLFWwindow* window, int button, int action, int mods);
static void viewScroll(GLFWwindow* window, double xoffset, double yoffset);
static void processCursorTrace(GLFWwindow* window,double xpos, double ypos);

void addPoint(vertexArray& array,const GLdouble cursorX,const GLdouble cursorY){
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat normalX = windowPara.screen2normalX(cursorX);
    const GLfloat normalY = windowPara.screen2normalY(cursorY);
    const GLfloat x = windowPara.normal2orthoX(normalX);
    const GLfloat y = windowPara.normal2orthoY(normalY);
    array.push_back(x);
    array.push_back(y);
    array.push_back(0.0f); // flat draw
    //std::cout<<"add control point"<<std::endl;
}
void addPoint(vertexArray& array,const GLfloat orthoX, const GLfloat orthoY){
    array.push_back(orthoX);
    array.push_back(orthoY);
    array.push_back(0.0f); // flat draw
}
void addPoint(vertexArray& array,const GLfloat orthoX, const GLdouble cursorY){
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat normalY = windowPara.screen2normalY(cursorY);
    const GLfloat y = windowPara.normal2orthoY(normalY);
    array.push_back(orthoX);
    array.push_back(y);
    array.push_back(0.0f); // flat draw
}
void addPoint(vertexArray& array,const GLdouble cursorX, const GLfloat orthoY){
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat normalX = windowPara.screen2normalX(cursorX);
    const GLfloat x = windowPara.normal2orthoX(normalX);
    array.push_back(x);
    array.push_back(orthoY);
    array.push_back(0.0f); // flat draw
}
void addBlock(vertexArray& array,const GLfloat orthoX, const GLfloat orthoY,GLfloat thickness){
    const int range = (thickness + 0.5)/2;
    array.push_back(orthoX);
    array.push_back(orthoY);
    array.push_back(0.0f);
    const int resolution = 10;
    for (int i = -range * resolution ; i<= range * resolution ; i++)
        for (int j = -range * resolution ; j<=range * resolution ; j++){
            array.push_back(orthoX+i/resolution);
            array.push_back(orthoY+j/resolution);
            array.push_back(0.0f); // flat draw
        }
}
static void DDA(vertexArray& array,float sX,float sY,float tX,float tY,bool antialising){
    float dx = tX - sX,dy = tY - sY;
    int steps = 0;
    if (std::abs(dx) >= std::abs(dy)){
        steps = std::abs(dx) * WindowParas::getInstance().xScale *2;
    }
    else{
        steps = std::abs(dy) * WindowParas::getInstance().yScale *2;
    }
    float xinc = dx / steps;
    float yinc = dy / steps;
    float x = sX,y = sY;
    //std::cout<<x<<y<<std::endl;
    const ImVec4 color = ShaderStyle::getStyle().drawColor;
    //std::cout<<color.x<<' '<<color.y<<' '<<color.z<<' '<<color.w<<std::endl;
    for (int i = 0; i <= steps; i++) {
        x += xinc;
        y += yinc;
        array.push_back(x);
        array.push_back(y);
        array.push_back(0.0f);
        if (antialising){
            array.push_back(1.0f);
            array.push_back(color.x);
            array.push_back(color.y);
            array.push_back(color.z);
            array.push_back(color.w);
        }
    }
}
static void drawLine(vertexArray& array,const GLfloat &sX,const GLfloat &sY,const GLdouble &cursorX,const GLdouble &cursorY,bool antialising) {
    const GLfloat thickness = ShaderStyle::getStyle().thickness;
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat normalX = windowPara.screen2normalX(cursorX);
    const GLfloat normalY = windowPara.screen2normalY(cursorY);
    const GLfloat tX = windowPara.normal2orthoX(normalX);
    const GLfloat tY = windowPara.normal2orthoY(normalY);
    //std::cout<<"this is right."<<sX << ' '<<sY<<' '<<tX << ' '<<tY<<std::endl;
    const int range = (thickness + 0.5)/2;
    for (int i = -range; i<= range; i++)
        for (int j = -range; j<=range; j++)
            DDA(array,sX+i,sY+j,tX+i,tY+j,antialising);
}

void keyBasicCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    MeauCallback(window, key, scancode, action, mods);
    keyModsToggle(window, key, scancode, action, mods);
    //process view move
    /*
    Records& record = Records::getState();
    double xpos,ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    const GLfloat cameraSpeed = Camera2D::getView().getCameraSpeed(400.0f);
    if (record.keyRecord[GLFW_KEY_W])
        ypos += cameraSpeed;
    if (record.keyRecord[GLFW_KEY_S])
        ypos -= cameraSpeed;
    if (record.keyRecord[GLFW_KEY_A])
        xpos += cameraSpeed;
    if (record.keyRecord[GLFW_KEY_D])
        xpos -= cameraSpeed;
    glfwSetCursorPos(window, xpos, ypos);
     */
    return;
}
void mouseDrawCallback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    mouseModsToggle(window, button, action, mods);
    drawModsToggle(window, button, action, mods);
    //tackle ondraw behavior
    Records& record = Records::getState();
    if (!Take::holdon().holdonToDraw && action == GLFW_PRESS && record.drawingPrimitive && record.pressLeft){
        GLdouble cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        addPoint(Take::holdon().drawingVertices,cursorX,cursorY);
    }
    //holdon draw method will not add middle points, which means the primitives drawed by this way has only two control points. So this situation is handled in cursorcallback
    return;
}
void mouseViewCallback(GLFWwindow* window, int button, int action, int mods){
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    mouseModsToggle(window, button, action, mods);
    return;
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    
    viewScroll(window,xoffset,yoffset);
    return;
}
void cursorSelectCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    
    return;
}
void windowPosCallback(GLFWwindow* window, int xpos, int ypos){
    gui::spiltUI();
}
void windowSizeCallback(GLFWwindow* window, int width, int height){
    gui::spiltUI();
}

void cursorDrawCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    processCursorTrace(window,xpos,ypos);
    return;
}
void cursorFocusCallback(GLFWwindow* window, int entered){
    ImGui_ImplGlfw_CursorEnterCallback(window, entered);
    return;
}

void MeauCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)){
        if (mods & GLFW_MOD_SHIFT){
            if (key == GLFW_KEY_Z){
                //redo
            }
            if (key == GLFW_KEY_EQUAL){
                //zoom in
            }
        }
        else{
        switch (key) {
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_N:
                //new file
                break;
            case GLFW_KEY_O:
                //open file
                break;
            case GLFW_KEY_S:
                //save file
                break;
            case GLFW_KEY_W:
                //export image
                break;
            case GLFW_KEY_Z:
                //undo
                break;
            case GLFW_KEY_A:
                //add data
                break;
            case GLFW_KEY_F:
                //zoom to layer
                break;
            case GLFW_KEY_F5:
                //switch to drag
                break;
            case GLFW_KEY_F6:
                //switch to select
                break;
            case GLFW_KEY_MINUS:
                //zoom out
                break;
            default:
                break;
        }
        }
    }
}
void keyModsToggle(GLFWwindow* window, int key, int scancode, int action, int mods){
    Records& record = Records::getState();
    if (action == GLFW_PRESS){
        //std::cout<<"press "<<key<<std::endl;
        record.keyRecord[key] = GL_TRUE;
        if (record.keyRecord[GLFW_KEY_LEFT_CONTROL] || record.keyRecord[GLFW_KEY_RIGHT_CONTROL]) record.pressCtrl = GL_TRUE;
        if (record.keyRecord[GLFW_KEY_LEFT_SHIFT] || record.keyRecord[GLFW_KEY_RIGHT_SHIFT])  record.pressShift = GL_TRUE;
        if (record.keyRecord[GLFW_KEY_LEFT_ALT] || record.keyRecord[GLFW_KEY_RIGHT_ALT])    record.pressAlt = GL_TRUE;
        
    }
    if (action == GLFW_RELEASE){
        //std::cout<<"release "<<key<<std::endl;
        record.keyRecord[key] = GL_FALSE;
        if (!record.keyRecord[GLFW_KEY_LEFT_CONTROL] && !record.keyRecord[GLFW_KEY_RIGHT_CONTROL]) record.pressCtrl = GL_FALSE;
        if (!record.keyRecord[GLFW_KEY_LEFT_SHIFT] && !record.keyRecord[GLFW_KEY_RIGHT_SHIFT])  record.pressShift = GL_FALSE;
        if (!record.keyRecord[GLFW_KEY_LEFT_ALT] && !record.keyRecord[GLFW_KEY_RIGHT_ALT])    record.pressAlt = GL_FALSE;
    }
}
void mouseModsToggle(GLFWwindow* window, int button, int action, int mods){
    Records& record = Records::getState();
    if (action == GLFW_PRESS){
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            record.pressLeft = GL_TRUE;
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
            record.pressRight = GL_TRUE;
    }
    if (action == GLFW_RELEASE){
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            record.pressLeft = GL_FALSE;
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
            record.pressRight = GL_FALSE;
    }
}
void viewScroll(GLFWwindow* window, double xoffset, double yoffset){
    Camera2D::getView().processScroll(window, xoffset, yoffset, Records::getState().pressCtrl, Records::getState().pressAlt);
}
static bool startDrawCheck(GLFWwindow* window, int button, int action, int mods){
    Records& record = Records::getState();
    if (!WindowParas::getInstance().mainWindowFocused)  return false; // the start points must in the window range
    if (action != GLFW_PRESS || !record.pressLeft) return false; // check left click
    if ( record.state != interectState::drawing || Take::holdon().drawType == Shape::NONE) return false; //check ready to start
    return !record.drawingPrimitive; //check aleady started
}
static bool finishDrawCheck(GLFWwindow* window, int button, int action, int mods){
    Records& record = Records::getState();
    if (!record.drawingPrimitive) return false; //check under drawing
    if (action == GLFW_PRESS){//click right to stop
        if (button == GLFW_MOUSE_BUTTON_RIGHT && !Take::holdon().holdonToDraw)
            return true;
        return false;
    }
    else if (action == GLFW_RELEASE){//release left to stop
        if (button == GLFW_MOUSE_BUTTON_LEFT && Take::holdon().holdonToDraw)
            return true;
        return false;
    }
    return false;
}
void drawModsToggle(GLFWwindow* window, int button, int action, int mods){
    Records& record = Records::getState();
    Take& take = Take::holdon();
    if (startDrawCheck(window, button, action, mods)){
        record.drawingPrimitive = true;
        //generate a new primitive
        take.drawingVertices.clear();
        std::cout<<"start draw"<<std::endl;
        GLdouble cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        addPoint(take.drawingVertices,cursorX,cursorY);
        if (take.drawType == Shape::LINES){
            const ImVec4 color = ShaderStyle::getStyle().drawColor;
            take.drawingVertices.push_back(1.0f);
            take.drawingVertices.push_back(color.x);
            take.drawingVertices.push_back(color.y);
            take.drawingVertices.push_back(color.z);
            take.drawingVertices.push_back(color.w);
        }
    }
    if (finishDrawCheck(window,button,action,mods)){
        record.drawingPrimitive = false;
        //finish the draw and push into the formal primitive render queue
        if (take.holdonToDraw){
            GLdouble cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            const GLfloat startX = take.drawingVertices[0],startY = take.drawingVertices[1];
            if (take.drawType == Shape::LINES){
                //std::cout<<"draw line"<<std::endl;
                //addPoint(Take::holdon().drawingVertices,cursorX,cursorY);
                drawLine(Take::holdon().drawingVertices,startX,startY,cursorX,cursorY,true);
            }
            else if (take.drawType == Shape::RECTANGLE){
                //vertexArray::const_reverse_iterator it = take.drawingVertices.rbegin();
                //const GLfloat startX = *(it+2),startY = *(it+1);
                addPoint(Take::holdon().drawingVertices,cursorX, startY);
                addPoint(Take::holdon().drawingVertices,cursorX, cursorY);
                addPoint(Take::holdon().drawingVertices,startX, cursorY);
            }
        }
        std::cout<<"finish draw"<<std::endl;
        Take& take = Take::holdon();
        ShaderStyle& style = ShaderStyle::getStyle();
        if (take.drawType == Shape::LINES){
            pPrimitive newPrimitive (new Primitive(take.drawingVertices, take.drawType, 8));
            pShader newShader(new Shader(style));
            newShader->attchVertexShader(rd::filePath("coloredVertices.vs"));
            newShader->attchFragmentShader(rd::filePath("transColor.frag"));
            newShader->linkProgram();
            rd::mainShaderList.push_back(std::move(newShader));
            newPrimitive->bindShader(rd::mainShaderList.back().get());
            pr::mainPrimitiveList.push_back(std::move(newPrimitive));
        }
        else{
            pPrimitive newPrimitive (new Primitive(take.drawingVertices, take.drawType, 3));
            pShader newShader(new Shader(style));
            newShader->attchVertexShader(rd::filePath("singleVertices.vs"));
            newShader->attchFragmentShader(rd::filePath("fillColor.frag"));
            newShader->linkProgram();
            rd::mainShaderList.push_back(std::move(newShader));
            newPrimitive->bindShader(rd::mainShaderList.back().get());
            pr::mainPrimitiveList.push_back(std::move(newPrimitive));
        }
        take.drawType = Shape::NONE;
    }
}

static Shape mapPreviewStyle(Shape drawType){
    if (drawType == Shape::RECTANGLE || drawType == Shape::POLYGEN || drawType ==Shape::TRIANGLE)
        return Shape::LOOP;
    return drawType;
}
void processCursorTrace(GLFWwindow* window,double xpos, double ypos){
    if (Records::getState().drawingPrimitive){//generate preview
        
        //take the last point
        //WindowParas& windowPara = WindowParas::getInstance();
        Take& take = Take::holdon();
        vertexArray tempVertices;
        const GLfloat startX = take.drawingVertices[0],startY = take.drawingVertices[1];
        
        if (take.holdonToDraw){
            if (take.drawType == Shape::LINES){
                //std::cout<<"draw line"<<std::endl;
                //addPoint(tempVertices,xpos,ypos);
                //addBlock(tempVertices,xpos,ypos,ShaderStyle::getStyle().thickness);
                drawLine(tempVertices,startX,startY,xpos,ypos,false);
            }
            else if (take.drawType == Shape::RECTANGLE){
                addPoint(tempVertices,startX,startY);
                //std::cout<<"draw rectangle"<<std::endl;
                addPoint(tempVertices,xpos, startY);
                addPoint(tempVertices,xpos, ypos);
                addPoint(tempVertices,startX, ypos);
            }
        }
        else{
            tempVertices = take.drawingVertices;
            addPoint(tempVertices,xpos,ypos);
        }
            
        //generate preview primitive
        pPrimitive previewPrimitive(new Primitive(tempVertices,mapPreviewStyle(Take::holdon().drawType),3));
        previewPrimitive -> bindShader(rd::namedShader["singleWhite"].get());
        pr::drawPreviewPrimitive = std::move(previewPrimitive);
    }
    else
        pr::drawPreviewPrimitive = nullptr;
    return;
}

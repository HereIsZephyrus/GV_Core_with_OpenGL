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
#include "shape.hpp"

void Records::initIObuffer(){
    memset(keyRecord, GL_FALSE, sizeof(keyRecord));
    pressLeft = GL_FALSE;
    pressRight = GL_FALSE;
    pressAlt = GL_FALSE;
    pressShift = GL_FALSE;
    pressCtrl = GL_FALSE;
    dragingMode = GL_FALSE;
    drawingPrimitive = GL_FALSE;
    showAxis = GL_TRUE;
    cliping = GL_FALSE;
    draging = GL_FALSE;
    state = interectState::toselect;
    primitiveList.clear();
}

static void MeauCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void keyModsToggle(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouseModsToggle(GLFWwindow* window, int button, int action, int mods);
static void cursorDragingDetect(GLFWwindow* window, double xpos, double ypos);
//static void cursorWindowMoveDetect(GLFWwindow* window, double xpos, double ypos);
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
    if (Records::getState().dragingMode){
        // drag to move window
        WindowParas& windowPara = WindowParas::getInstance();
        if (button == GLFW_MOUSE_BUTTON_LEFT){
            if (action == GLFW_PRESS){
                Records::getState().draging = GL_TRUE;
                GLdouble xpos,ypos;
                Records& record = Records::getState();
                glfwGetCursorPos(window, &xpos, &ypos);
                const GLfloat cursorX =windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
                const GLfloat cursorY =windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
                record.previewXpos = cursorX;
                record.previewYpos = cursorY;
                record.previewPosition = Camera2D::getView().getPosition();
                glfwSetCursorPosCallback(window, cursorDragCallback);
            }
            else if (action == GLFW_RELEASE){
                Records::getState().draging = GL_FALSE;
                glfwSetCursorPosCallback(window, cursorSelectCallback);
            }
        }
        
    }{
        //mouse track to select primitives
    }
    return;
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    
    viewScroll(window,xoffset,yoffset);
    return;
}
void cursorDefaultCallback(GLFWwindow* window, double xpos, double ypos){
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
void cursorDragCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    cursorDragingDetect(window,xpos,ypos);
    return;
}
void cursorSelectCallback(GLFWwindow* window, double xpos, double ypos){
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    //cursorWindowMoveDetect(window,xpos,ypos);
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
    coord::generateCoordinateAxis();
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
        std::cout<<"start draw"<<std::endl;
        take.drawingVertices.clear();
        GLdouble cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        addPoint(Take::holdon().drawingVertices,cursorX,cursorY);
    }
    if (finishDrawCheck(window,button,action,mods)){
        record.drawingPrimitive = false;
        //finish the draw and push into the formal primitive render queue
        if (take.holdonToDraw){
            GLdouble cursorX, cursorY;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            if (take.drawType == Shape::LINES){
                //std::cout<<"draw line"<<std::endl;
                addPoint(Take::holdon().drawingVertices,cursorX,cursorY);
            }
            else if (take.drawType == Shape::RECTANGLE || take.drawType == Shape::LOOP){
                vertexArray::const_reverse_iterator it = take.drawingVertices.rbegin();
                const GLfloat startX = *(it+2),startY = *(it+1);
                std::cout<<std::endl;
                addPoint(Take::holdon().drawingVertices,cursorX, startY);
                addPoint(Take::holdon().drawingVertices,cursorX, cursorY);
                addPoint(Take::holdon().drawingVertices,startX, cursorY);
            }
        }
        std::cout<<"finish draw"<<std::endl;
        Take& take = Take::holdon();
        //ShaderStyle& style = ShaderStyle::getStyle();
        pPrimitive newPrimitive (new Primitive(take.drawingVertices, take.drawType, 3));
        pShader newShader(new Shader());
        newShader->attchVertexShader(rd::filePath("singleVertices.vs"));
        //newShader->attchVertexShader(rd::filePath("lineWidth.frag"));
        newShader->attchFragmentShader(rd::filePath("fillColor.frag"));
        newShader->linkProgram();
        rd::mainShaderList.push_back(std::move(newShader));
        if (!record.cliping){
            newPrimitive->bindShader(rd::mainShaderList.back().get());
            pr::mainPrimitiveList.push_back(std::move(newPrimitive));
            Primitive* lastpPrimitive = pr::mainPrimitiveList.back().get();
            createTopoElements(lastpPrimitive);
            record.primitiveList.emplace_back(std::make_pair(lastpPrimitive, std::string("primitive") + std::to_string(lastpPrimitive->layer)));
        }
        else{
            take.clipShape  =std::move(newPrimitive);
            clipByShape();
            take.clipShape = nullptr;
        }
        take.drawType = Shape::NONE;
    }
}
void cursorDragingDetect(GLFWwindow* window,double xpos, double ypos){
    Records& record = Records::getState();
    WindowParas& windowPara = WindowParas::getInstance();
    if (record.draging){
        const GLfloat cursorX =windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
        const GLfloat cursorY =windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
        const GLfloat dX = (record.previewXpos - cursorX)/windowPara.xScale ;
        const GLfloat dY = (record.previewYpos - cursorY)/windowPara.yScale ;
        Camera2D::getView().setDeltaPosition(record.previewPosition,dX,dY);
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
        vertexArray::const_reverse_iterator it = Take::holdon().drawingVertices.rbegin();
        const GLfloat startX = *(it+2),startY = *(it+1);
        addPoint(tempVertices,startX,startY);
        if (take.holdonToDraw){
            if (take.drawType == Shape::LINES){
                //std::cout<<"draw line"<<std::endl;
                addPoint(tempVertices,xpos,ypos);
            }
            else if (take.drawType == Shape::RECTANGLE || take.drawType == Shape::LOOP){
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
        previewPrimitive -> bindShader(rd::namedShader["previewShader"].get());
        pr::drawPreviewPrimitive = std::move(previewPrimitive);
    }
    else
        pr::drawPreviewPrimitive = nullptr;
    return;
}

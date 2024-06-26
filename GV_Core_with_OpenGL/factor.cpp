//
//  factor.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/26/24.
//

#include "commander.hpp"
#include "window.hpp"
#include "primitive.hpp"
#include "camera.hpp"
#include "shape.hpp"
#include "factor.hpp"

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
        const GLfloat dX = (record.previewXpos - cursorX);
        const GLfloat dY = (record.previewYpos - cursorY);
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
/*
bool cursorDetectPoint(std::shared_ptr<pr::Point> element,double xpos, double ypos){
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    const GLfloat& pointX = (*element->getVertexArray())[element->vertexIndex[0]];
    const GLfloat pointDetectRange = 3.0f;
    if ((cursorX - element.))
}
void cursorDetectLine(std::shared_ptr<pr::Line> element,double xpos, double ypos){
    
}
void cursorDetectFace(std::shared_ptr<pr::Face> element,double xpos, double ypos){
    
}
*/

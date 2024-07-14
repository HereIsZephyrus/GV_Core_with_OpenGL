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
#include "objectmodel.hpp"

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
    if ( record.state != interectState::drawing || Take::holdon().drawType == Shape::NONE ) return false; //check ready to start
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
    Take& take = Take::holdon();
    if (startDrawCheck(window, button, action, mods)){
        Records::getState().drawingPrimitive = true;
        std::cout<<"start draw"<<std::endl;
        take.drawingVertices.clear();
        GLdouble cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        //generate the first point to the new primitive
        if (take.holdonToDraw)
            addPoint(Take::holdon().drawingVertices,cursorX,cursorY);
    }
    if (finishDrawCheck(window,button,action,mods)){
        Records::getState().drawingPrimitive = false;
        //finish the primitive
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
                addPoint(Take::holdon().drawingVertices,cursorX, startY);
                addPoint(Take::holdon().drawingVertices,cursorX, cursorY);
                addPoint(Take::holdon().drawingVertices,startX, cursorY);
            }
        }
        std::cout<<"finish draw"<<std::endl;
        // push the primitive into the formal primitive render queue
        generateNewPrimitive();
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
    if (drawType == Shape::CURVE)
        return Shape::POINTS;
    if (drawType == Shape::MARKER)
        return Shape::POINTS;
    return drawType;
}
void processCursorTrace(GLFWwindow* window,double xpos, double ypos){
    Take& take = Take::holdon();
    if (Records::getState().drawingPrimitive && take.drawType != Shape::MARKER){//generate preview
        //take the last point
        //WindowParas& windowPara = WindowParas::getInstance();
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
        generatePreviewPrimitive(tempVertices);
    }
    else
        pr::previewPrimitive = nullptr;
    return;
}
void editPrimitive(){
    Take& take = Take::holdon();
    Records& record = Records::getState();
    WindowParas& windowPara = WindowParas::getInstance();
    GLdouble xpos,ypos;
    glfwGetCursorPos(windowPara.window, &xpos, &ypos);
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    if (record.pressLeft){
        std::cout<<"exert transfrom"<<std::endl;
        pElement outbound = take.editingPrimitive->elementList.back();
        take.transMat = glm::mat3(1.0f);
        int  relationship = outboundDetect(outbound);
        if (relationship == 0 || relationship == 1){//inside and on
            pElement holdonElement = nullptr;
            const GLfloat dX = (cursorX - record.previewXpos);
            const GLfloat dY = (cursorY - record.previewYpos);
            // to move
            take.transMat[2][0] = dX; take.transMat[2][1] = dY;
            //for the sake of topography build sequeence,the points push back first, then lines, then face. so when detect happend,it will be the fitest one
            for (auto element = take.editingPrimitive->elementList.begin(); element != take.editingPrimitive->elementList.end(); element++){
                if ((*element)->cursorSelectDetect(xpos, ypos)){
                    holdonElement = *element;
                    break;
                }
            }
            
            //std::cout<<"move:"<<dX<<' '<<dY<<std::endl;
            if (holdonElement != nullptr){
                //take.editingPrimitive->transform(holdonElement->getVertexIndex(), take.transMat);
                /*
                if (holdonElement->getShape() == GL_POINT || holdonElement->getShape() == GL_POINTS){
                    //to recognize center of the point set
                    take.editingPrimitive->transform(take.transMat);
                }else{
                    take.editingPrimitive->transform(holdonElement->getVertexIndex(), take.transMat);
                }*/
            }
            else{
                //no select,default move all
                //take.editingPrimitive->transform(take.transMat);
            }
        }else if (relationship ==2){
            //to rotate
            glm::mat3 move = glm::mat3(1.0f);
            const glm::vec2 rotateCenter = outbound->getRotateCenter();
            move[2][0] = rotateCenter.x; move[2][1] = rotateCenter.y;
            const glm::vec2 startVec = glm::vec2{record.previewXpos - rotateCenter.x,record.previewYpos - rotateCenter.y};
            const glm::vec2 endVec = glm::vec2{cursorX - rotateCenter.x,cursorY - rotateCenter.y};
            const GLfloat theta = glm::dot(startVec, endVec) / (glm::length(startVec) * glm::length(endVec));
            take.transMat[0][0] = glm::cos(theta); take.transMat[1][0] = glm::sin(-theta);
            take.transMat[0][1] = glm::sin(theta); take.transMat[1][1] = glm::cos(theta);
            take.transMat = move * take.transMat;
            move[2][0] = -rotateCenter.x; move[2][1] = -rotateCenter.y;
            take.transMat = take.transMat * move;
            }
        else if (relationship<=10){
            //to scale
            const GLfloat dX = abs(cursorX - record.previewXpos);
            const GLfloat dY = abs(cursorY - record.previewYpos);
            const pVertexArray& vertices = outbound->getVertexArray();
            
            glm::vec2 boundary[4]; //leftbottom - lefttop - righttop - rightbottom
            for (int i = 0; i<4; i++){
                GLint startIndex = outbound->getVertexIndex()[i];
                boundary[i] = {(*vertices)[startIndex],(*vertices)[startIndex+1]};
            }
            const GLfloat outboundX = abs(boundary[0].x - boundary[2].x),outboundY = abs(boundary[0].y - boundary[2].y);
            glm::mat3 move = glm::mat3(1.0f);
            if (record.pressCtrl){
                //to scale center
                const glm::vec2 geoCenter = outbound->getGeoCenter();
                move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                take.transMat[0][0] = dX; take.transMat[1][1] = dY;
                take.transMat = move * take.transMat;
                move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                take.transMat = take.transMat * move;
            }else{
                //warning: the scale factor(d/outboud) is not correct. It should be judged by the cursor move direction
                if (relationship == 3){
                    //to scale left
                    const glm::vec2 geoCenter = boundary[2];
                    move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                    take.transMat[0][0] = dX/outboundX;
                    take.transMat = move * take.transMat;
                    move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                    take.transMat = take.transMat * move;
                }
                else if (relationship == 4){
                    //to scale top
                    const glm::vec2 geoCenter = boundary[0];
                    move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                    take.transMat[1][1] = dY/outboundY;
                    take.transMat = move * take.transMat;
                    move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                    take.transMat = take.transMat * move;
                }
                else if (relationship == 5){
                    //to scale right
                    const glm::vec2 geoCenter = boundary[0];
                    move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                    take.transMat[0][0] = dX/outboundY;
                    take.transMat = move * take.transMat;
                    move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                    take.transMat = take.transMat * move;
                }
                else if (relationship == 6){
                    //to scale bottom
                    const glm::vec2 geoCenter = boundary[2];
                    move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                     take.transMat[1][1] = dY/outboundY;
                    take.transMat = move * take.transMat;
                    move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                    take.transMat = take.transMat * move;
                }
                else{
                    GLint index = relationship - 7;
                    const glm::vec2 geoCenter = boundary[index];
                    move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                    take.transMat[0][0] = dX/outboundX; take.transMat[1][1] = dY/outboundY;
                    take.transMat = move * take.transMat;
                    move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                    take.transMat = take.transMat * move;
                }
            }
        }else{
            //to sheer
            const GLfloat dX = abs(cursorX - record.previewXpos);
            const GLfloat dY = abs(cursorY - record.previewYpos);
            const pVertexArray& vertices = outbound->getVertexArray();
            
            glm::vec2 boundary[4]; //leftbottom - lefttop - righttop - rightbottom
            for (int i = 0; i<4; i++){
                GLint startIndex = outbound->getVertexIndex()[i];
                boundary[i] = {(*vertices)[startIndex],(*vertices)[startIndex+1]};
            }
            const GLfloat outboundX = abs(boundary[0].x - boundary[2].x),outboundY = abs(boundary[0].y - boundary[2].y);
            glm::mat3 move = glm::mat3(1.0f);
            if (relationship == 11){
                //to sheer left
                const GLfloat sheerX = dX/outboundX;
                const glm::vec2 geoCenter = boundary[2];
                move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                take.transMat[0][1] = sheerX; take.transMat[2][1] = -sheerX * geoCenter.y;
                take.transMat = move * take.transMat;
                move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                take.transMat = take.transMat * move;
            }
            else if (relationship == 12){
                //to sheer top
                const GLfloat sheerY = dY/outboundY;
                const glm::vec2 geoCenter = boundary[0];
                move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                take.transMat[0][1] = sheerY; take.transMat[2][1] = -sheerY * geoCenter.x;
                take.transMat = move * take.transMat;
                move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                take.transMat = take.transMat * move;
            }
            else if (relationship == 13){
                //to sheer right
                const GLfloat sheerX = dX/outboundX;
                const glm::vec2 geoCenter = boundary[0];
                move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                take.transMat[0][1] = sheerX; take.transMat[2][1] = -sheerX * geoCenter.y;
                take.transMat = move * take.transMat;
                move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                take.transMat = take.transMat * move;
            }
            else if (relationship == 14){
                //to sheer bottom
                const glm::vec2 geoCenter = boundary[2];
                const GLfloat sheerY = dY/outboundY;
                move[2][0] = geoCenter.x; move[2][1] = geoCenter.y;
                take.transMat[0][1] = sheerY; take.transMat[2][1] = -sheerY * geoCenter.x;
                take.transMat = move * take.transMat;
                move[2][0] = -geoCenter.x; move[2][1] = -geoCenter.y;
                take.transMat = take.transMat * move;
            }
        }
    }
}
void generateNewPrimitive(){
    Take& take = Take::holdon();
    Records& record = Records::getState();
    ShaderStyle& style = ShaderStyle::getStyle();
    if (take.drawType == Shape::MARKER){
        return;
    }
    pPrimitive newPrimitive (new Primitive(take.drawingVertices, take.drawType, 3));
    pShader newShader(new Shader());
    switch (take.drawType) {
        case Shape::LINES:
            newShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
            switch (style.lineType) {
                case LineType::fill:
                    newShader->attchShader(rd::filePath("fillColor.frag"),GL_FRAGMENT_SHADER);
                    switch (style.headType) {
                        case LineHeadType::cube:
                            newShader->attchShader(rd::filePath("cubeLine.gs"), GL_GEOMETRY_SHADER);
                            break;
                        case LineHeadType::circle:
                            newShader->attchShader(rd::filePath("circleLine.gs"), GL_GEOMETRY_SHADER);
                            break;
                        default:
                            break;
                    }
                    break;
                case LineType::dot:
                    newShader->attchShader(rd::filePath("dotColor.frag"),GL_FRAGMENT_SHADER);
                    break;
                case LineType::semi:
                    newShader->attchShader(rd::filePath("semiColor.frag"),GL_FRAGMENT_SHADER);
                    break;
                default:
                    newShader->attchShader(rd::filePath("fillColor.frag"),GL_FRAGMENT_SHADER);
                    break;
            }
            break;
        case Shape::CIRCLE:
            newShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
            //if (style.toFill)
                newShader->attchShader(rd::filePath("fillCircle.gs"), GL_GEOMETRY_SHADER);
            newShader->attchShader(rd::filePath("fillColor.frag"),GL_FRAGMENT_SHADER);
            break;
        default:
            newShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
            newShader->attchShader(rd::filePath("fillColor.frag"), GL_FRAGMENT_SHADER);
            break;
    }
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
        take.clipShape =std::move(newPrimitive);
        clipByShape();
        take.clipShape = nullptr;
    }
}
void generatePreviewPrimitive(const vertexArray& tempVertices){
    Take& take = Take::holdon();
    pPrimitive previewPrimitive(new Primitive(tempVertices,mapPreviewStyle(take.drawType),3));
    if (take.drawType == Shape::LINES){
        ShaderStyle& style = ShaderStyle::getStyle();
        switch (style.headType) {
            case LineHeadType::cube:
                previewPrimitive -> bindShader(rd::namedShader["previewCubeLineShader"].get());
                break;
            case LineHeadType::circle:
                previewPrimitive -> bindShader(rd::namedShader["previewCircleLineShader"].get());
                break;
            default:
                break;
        }
    }
    else
        previewPrimitive -> bindShader(rd::namedShader["previewfillShader"].get());
    pr::previewPrimitive = std::move(previewPrimitive);
}

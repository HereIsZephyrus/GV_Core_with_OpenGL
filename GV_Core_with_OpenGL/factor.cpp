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
void toAlignment(vertexArray& array,Shape shape){
    const GLsizei stride = 3;
    if (shape == Shape::RECTANGLE || shape == Shape::CIRCLE){
        const GLfloat dx = array[stride] - array[0];
        const bool isNegtive = (array[stride + 1] - array[1]) < 0;
        GLfloat dy = dx ;
        if (isNegtive)   dy = -dy;
        if (dx < 0)         dy = -dy;
        array[stride+1] = array[1] + dy;
    }
}
void outboundInterectCheck(pOutbound outbound){
    Records& record = Records::getState();
    WindowParas& windowPara = WindowParas::getInstance();
    Take& take = Take::holdon();
    GLdouble xpos,ypos;
    glfwGetCursorPos(windowPara.window, &xpos, &ypos);
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    if (record.pressLeft && windowPara.mainWindowFocused){
        glm::mat4 newTransMat = glm::mat4(1.0f);
        int relationship = outbound->relationship;
        const GLfloat dX = (cursorX - record.previewXpos);
        const GLfloat dY = (cursorY - record.previewYpos);
        const glm::mat4 exertMat = outbound->getTransmat(),primitiveMat = take.editingPrimitive->getTransMat();
        glm::vec4 raw_leftbottomPoint = exertMat * primitiveMat * glm::vec4(outbound->getMinX(),outbound->getMinY(),0.0f,1.0f);
        glm::vec4 raw_righttopPoint = exertMat * primitiveMat * glm::vec4(outbound->getMaxX(),outbound->getMaxY(),0.0f,1.0f);
        const GLfloat minX = raw_leftbottomPoint.x,minY = raw_leftbottomPoint.y;
        const GLfloat maxX = raw_righttopPoint.x,maxY = raw_righttopPoint.y;
        //glm::vec2 size = {maxX - minX,maxY - minY};
        glm::vec2 size = outbound->getSize();
        if (relationship == 0){// 0000 inside to move
            std::cout<<"0000 inside to move"<<std::endl;
            newTransMat[3][0] = 1 + dX; newTransMat[3][1] = 1 + dY;
        }else if (relationship == 1){// 0001 left to sheer
            std::cout<<"0001 left to sheer"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat sheerX = dX/size.x;
            const GLfloat refX = minX, refY = outbound->getGeocenter().y;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[1][0] = sheerX; newTransMat[2][0] = -sheerX * refY;
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
        }else if (relationship == 2){// 0010 right to sheer
            std::cout<<"0010 right to sheer"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat sheerX = dX/size.x;
            const GLfloat refX = maxX, refY = outbound->getGeocenter().y;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[1][0] = sheerX; newTransMat[2][0] = -sheerX * refY;
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
        }else if (relationship == 4){// 0100 bottom to sheer
            std::cout<<"0100 bottom to sheer"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat sheerY = dY/size.y;
            const GLfloat refX = outbound->getGeocenter().x, refY = minY;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[0][1] = sheerY; newTransMat[2][1] = -sheerY * refX;
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
        }else if (relationship == 8){// 1000 top to sheer
            std::cout<<"1000 top to sheer"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat sheerY = dY/size.y;
            const GLfloat refX = outbound->getGeocenter().x, refY = maxY;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[0][1] = sheerY; newTransMat[2][1] = -sheerY * refX;
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
        }else if (relationship == 9 || relationship == 10 || relationship == 5 || relationship == 6){
            //1001 1010 0110 0101 four corners to rotate
            std::cout<<"1001 1010 0110 0101 four corners to rotate"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const glm::vec2 rotateCenter = outbound->getRotateCenter();
            move[3][0] = rotateCenter.x; move[3][1] = rotateCenter.y;
            const glm::vec2 startVec = glm::vec2{record.previewXpos - rotateCenter.x,record.previewYpos - rotateCenter.y};
            const glm::vec2 endVec = glm::vec2{cursorX - rotateCenter.x,cursorY - rotateCenter.y};
            const GLfloat theta = glm::dot(startVec, endVec) / (glm::length(startVec) * glm::length(endVec));
            newTransMat[0][0] = glm::cos(theta); newTransMat[1][0] = glm::sin(-theta);
            newTransMat[0][1] = glm::sin(theta); newTransMat[1][1] = glm::cos(theta);
            newTransMat = move * newTransMat;
            move[3][0] = -rotateCenter.x; move[3][1] = -rotateCenter.y;
            newTransMat = newTransMat * move;
        }else if (relationship == -1){// -0001 left to scale
            std::cout<<"-0001 left to scale"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat refX = maxX, refY = outbound->getGeocenter().y;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[0][0] = std::max(0.1f, 1 - dX/size.x);
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
            outbound->setRefLine(refX,refY);
        }else if (relationship == -2){// -0010 right to scale
            std::cout<<"-0010 right to scale"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat refX = minX, refY = outbound->getGeocenter().y;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[0][0] = std::min(10.0f, 1 + dX/size.x);
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
            outbound->setRefLine(refX,refY);
        }else if (relationship == -4){// -0100 bottom to scale
            std::cout<<"-0100 bottom to scale"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat refX = outbound->getGeocenter().x, refY = maxY;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[1][1] = std::max(0.1f, 1 - dY/size.y);
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
            outbound->setRefLine(refX,refY);
        }else if (relationship == -8){// -1000 top to scale
            std::cout<<"-1000 top to scale"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat refX = outbound->getGeocenter().x, refY = minY;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[1][1] = std::min(10.0f, 1 + dY/size.y);
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
            outbound->setRefLine(refX,refY);
        }else if (relationship == -9){// -1001 left-top to scale
            std::cout<<"-1001 left-top to scale"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat refX = maxX, refY = minY;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[0][0] = std::max(0.1f, 1 - dX/size.x);
            newTransMat[1][1] = std::min(10.0f, 1 + dY/size.y);
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
            outbound->setRefLine(refX,refY);
        }else if (relationship == -10){// -1010 right-top to scale
            std::cout<<"-1010 right-top to scale"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat refX = minX, refY = minY;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[0][0] = std::min(10.0f, 1 + dX/size.x);
            newTransMat[1][1] = std::min(10.0f, 1 + dY/size.y);
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
            outbound->setRefLine(refX,refY);
        }else if (relationship == -6){// -0110 right-bottom to scale
            std::cout<<"-0110 right-bottom to scale"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat refX = minX, refY = maxY;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[0][0] = std::min(10.0f, 1 + dX/size.x);
            newTransMat[1][1] = std::max(0.1f, 1 - dY/size.y);
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
            outbound->setRefLine(refX,refY);
        }else if (relationship == -5){// -0101 left-bottom to scale
            std::cout<<"-0101 left-bottom to scale"<<std::endl;
            glm::mat4 move = glm::mat4(1.0f);
            const GLfloat refX = maxX, refY = maxY;
            move[3][0] = refX; move[3][1] = refY;
            newTransMat[0][0] = std::max(0.1f, 1 - dX/size.x);
            newTransMat[1][1] = std::max(0.1f, 1 - dY/size.y);
            newTransMat = move * newTransMat;
            move[3][0] = -refX; move[3][1] = -refY;
            newTransMat = newTransMat * move;
            outbound->setRefLine(refX,refY);
        }
        outbound->setTransmat(newTransMat);
    }
}
void generateNewPrimitive(){
    Take& take = Take::holdon();
    Records& record = Records::getState();
    ShaderStyle& style = ShaderStyle::getStyle();
    if (take.drawType == Shape::MARKER)
        return;
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
            if (style.toFill)
                newShader->attchShader(rd::filePath("fillCircle.gs"), GL_GEOMETRY_SHADER);
            else
                newShader->attchShader(rd::filePath("lineCircle.gs"), GL_GEOMETRY_SHADER);
            newShader->attchShader(rd::filePath("fillColor.frag"),GL_FRAGMENT_SHADER);
            break;
        case Shape::RECTANGLE:
            newShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
            if (style.toFill)
                newShader->attchShader(rd::filePath("fillRect.gs"), GL_GEOMETRY_SHADER);
            else
                newShader->attchShader(rd::filePath("lineRect.gs"), GL_GEOMETRY_SHADER);
            newShader->attchShader(rd::filePath("fillColor.frag"),GL_FRAGMENT_SHADER);
            break;
        case Shape::POLYGEN:
            newShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
            if (!style.toFill)
                newShader->attchShader(rd::filePath("circleLine.gs"), GL_GEOMETRY_SHADER);
            newShader->attchShader(rd::filePath("fillColor.frag"), GL_FRAGMENT_SHADER);
            break;
        case Shape::CURVE:
            newShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
            if (!style.toFill)
                newShader->attchShader(rd::filePath("bezierCurve.gs"), GL_GEOMETRY_SHADER);
            newShader->attchShader(rd::filePath("fillColor.frag"), GL_FRAGMENT_SHADER);
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
        //std::cout<<lastpPrimitive->getVertexNum()<<std::endl;
        generateNewPrimitiveList(take.drawType,lastpPrimitive);
    }
    else{
        take.clipShape =std::move(newPrimitive);
        clipByShape();
        take.clipShape = nullptr;
    }
}

static Shape mapPreviewStyle(Shape drawType){
    if (drawType == Shape::POLYGEN)
        return Shape::LOOP;
    if (drawType == Shape::CURVE)
        return Shape::POINTS;
    if (drawType == Shape::MARKER)
        return Shape::POINTS;
    return drawType;
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
    else if (take.drawType == Shape::RECTANGLE)
        previewPrimitive -> bindShader(rd::namedShader["previewRectangleShader"].get());
    else if (take.drawType == Shape::CIRCLE)
        previewPrimitive -> bindShader(rd::namedShader["previewCircleShader"].get());
    else if (take.drawType == Shape::POLYGEN && !ShaderStyle::getStyle().toFill)
        previewPrimitive -> bindShader(rd::namedShader["previewCircleLineShader"].get());
    else
        previewPrimitive -> bindShader(rd::namedShader["previewfillShader"].get());
    pr::previewPrimitive = std::move(previewPrimitive);
}
void generateNewPrimitiveList(Shape shape,Primitive* primitive){
    const int id = static_cast<int>(shape);
    gui::itemInfo[id].count++;
    Records& record = Records::getState();
    primitive->priority = static_cast<GLuint>(record.primitiveList.size())+1;
    record.primitiveList.emplace_back(std::make_shared<Item>(primitive, gui::itemInfo[id].typeName + std::to_string(gui::itemInfo[id].count)));
    Take::holdon().activeLayer->itemlist.push_back(record.primitiveList.back());
}
void upstreamStatus(){
    Records& record = Records::getState();
    Take& take = Take::holdon();
    if (record.state != interectState::holding && record.state != interectState::editing)
        gui::lastState = record.state;
    else if (gui::lastState == interectState::none)
        gui::lastState = interectState::toselect;
    if (record.state != interectState::drawing && take.activeLayer != nullptr){ //stop without finish primitive
        if (take.activeLayer->itemlist.empty())
            record.layerList.pop_back();
        take.activeLayer = nullptr;
    }
}

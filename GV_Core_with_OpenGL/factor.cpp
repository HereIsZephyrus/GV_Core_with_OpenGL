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
        //record.primitiveList.emplace_back(std::make_pair(lastpPrimitive, std::string("primitive") + std::to_string(lastpPrimitive->layer)));
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
    record.primitiveList.emplace_back(Item(primitive, gui::itemInfo[id].typeName + std::to_string(gui::itemInfo[id].count)));
    Take::holdon().activeLayer->itemlist.push_back(std::make_shared<Item>(record.primitiveList.back()));
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

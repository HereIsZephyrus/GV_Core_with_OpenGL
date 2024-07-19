//
//  shape.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/16/24.
//

#include "shape.hpp"
#include "camera.hpp"
#include "window.hpp"
namespace pr{
void Element::setColor(bool highlighted){
    //color
    GLuint colorLoc = glGetUniformLocation(shader->program,"setColor");
    GLuint brightLoc = glGetUniformLocation(shader->program,"brightness");
    glUniform4f(colorLoc,style.color.x,style.color.y,style.color.z,style.color.w);
    glUniform1f(brightLoc,1+0.2f*static_cast<int>(highlighted));
}
void Point::draw(bool highlighted){
    setColor(highlighted);
    //thickness
    const GLfloat zoom = Camera2D::getView().getZoom();
    GLuint sizeLoc = glGetUniformLocation(shader->program,"thickness");
    glUniform1f(sizeLoc,pointSize / zoom);
    // load data
    glBindVertexArray(identifier->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(shape,static_cast<GLsizei>(vertexIndex.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return;
}
void Line::draw(bool highlighted){
    setColor(highlighted);
    //thickness
    const GLfloat zoom = Camera2D::getView().getZoom();
    GLuint sizeLoc = glGetUniformLocation(shader->program,"thickness");
    glUniform1f(sizeLoc,lineWidth / zoom);
    // load data
    glBindVertexArray(identifier->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(shape,static_cast<GLsizei>(vertexIndex.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return;
}
void Face::draw(bool highlighted){
    setColor(highlighted);
    //thickness
    GLuint sizeLoc = glGetUniformLocation(shader->program,"thickness");
    glUniform1f(sizeLoc,2.0f);
    // load data
    glBindVertexArray(identifier->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(shape,static_cast<GLsizei>(vertexIndex.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return;
}
void Curve::draw(bool highlighted){
    setColor(highlighted);
    //thickness
    const GLfloat zoom = Camera2D::getView().getZoom();
    GLuint sizeLoc = glGetUniformLocation(shader->program,"thickness");
    glUniform1f(sizeLoc,lineWidth / zoom);
    //std::cout<<"print curve"<<std::endl;
    // load data
    glBindVertexArray(identifier->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(shape,static_cast<GLsizei>(vertexIndex.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return;
}
void OutBound::draw(bool highlighted){
    
    return;
}
void Diagnoal::draw(bool highlighted){
    setColor(highlighted);
    //thickness
    const GLfloat zoom = Camera2D::getView().getZoom();
    GLuint sizeLoc = glGetUniformLocation(shader->program,"thickness");
    glUniform1f(sizeLoc,lineWidth / zoom);
    // load data
    glBindVertexArray(identifier->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(shape,static_cast<GLsizei>(vertexIndex.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return;
}
void updateIndex(Primitive* primitive){
    const primitiveIdentifier* identifier = primitive->getIdentifier();
    primitive->elementList.erase(std::remove_if(primitive->elementList.begin(), primitive->elementList.end(),
                                         [identifier](const pElement&  x){
                                            return x->getIdentifier()->VAO == identifier->VAO;
                                            }),
                          primitive->elementList.end());
    updateTopoElements(primitive);
}
bool Point::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    WindowParas& windowPara = WindowParas::getInstance();
    //std::cout<<"detect point"<<std::endl;
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    const GLfloat& pointX = (*refVertex)[vertexIndex[0] * stride],pointY = (*refVertex)[vertexIndex[0] * stride + 1];
    const GLfloat pointDetectRange = gui::detactBias * Camera2D::getView().getZoom();
    if ((cursorX - pointX) * (cursorX - pointX) + (cursorY - pointY) * (cursorY - pointY) <= pointDetectRange * pointDetectRange * pointSize * pointSize){
        return true;
    }
    return false;
}
GLfloat distancePointToLineSQ(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2, GLfloat x0, GLfloat y0) {
    const GLfloat m = (y2 - y1) / (x2 - x1);
    const GLfloat b = y1 - m * x1;
    const float a = std::abs(m * x0 - y0 + b);
    GLfloat distance = a * a / (m * m + 1);
    return distance;
}
bool Line::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    WindowParas& windowPara = WindowParas::getInstance();
    //std::cout<<"detect line"<<std::endl;
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    const GLfloat& pointX1 = (*refVertex)[vertexIndex[0] * stride],pointY1 = (*refVertex)[vertexIndex[0] * stride + 1];
    const GLfloat& pointX2 = (*refVertex)[vertexIndex[1] * stride],pointY2 = (*refVertex)[vertexIndex[1] * stride + 1];
    const GLfloat zoom = Camera2D::getView().getZoom();
    const GLfloat paralellRange = gui::detactBias * zoom;
    bool inTheRange = false,onTheSlop = false;
    if (abs(pointX2- pointX1)< paralellRange){
        inTheRange = (cursorY > pointY1) != (cursorY > pointY2);
        onTheSlop = (abs(pointX1 - cursorX)< paralellRange * lineWidth) || (abs(pointX2 - cursorX)< paralellRange * lineWidth);
    }else if (abs(pointY2 - pointY1)<paralellRange){
        inTheRange = (cursorX > pointX1) != (cursorX > pointX2);
        onTheSlop = (abs(pointY1 - cursorY)< paralellRange * lineWidth) || (abs(pointY2 - cursorY)< paralellRange * lineWidth);
    }else{
        inTheRange = ((cursorX > pointX1) != (cursorX > pointX2)) && (cursorY > pointY1) != (cursorY > pointY2);
        onTheSlop = distancePointToLineSQ(pointX1,pointY1,pointX2,pointY2,cursorX,cursorY)<= paralellRange * paralellRange* lineWidth * lineWidth;
    }
    if (inTheRange && onTheSlop)
        return true;
    return false;
}
bool Face::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    WindowParas& windowPara = WindowParas::getInstance();
    //std::cout<<"detect face"<<std::endl;
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    bool inside = false;
    for (auto it = line.begin(); it != line.end(); it ++){
        const GLuint ind1 = (*it)->vertexIndex[0] ,ind2 = (*it)->vertexIndex[1];
        const GLfloat& pointX1 = (*refVertex)[ind1 * stride],pointY1 = (*refVertex)[ind1 * stride + 1];
        const GLfloat& pointX2 = (*refVertex)[ind2* stride],pointY2 = (*refVertex)[ind2* stride + 1];
        if (((pointY1 > cursorY) != (pointY2 > cursorY)) && (cursorX < (pointX2 - pointX1) * (cursorY - pointY1) / (pointY2 - pointY1) + pointX1)){
            inside = !inside;
        }
    }
    return inside;
}
bool OutBound::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
   
    return false;
}
bool Curve::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    
    return false;
}
bool Diagnoal::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    WindowParas& windowPara = WindowParas::getInstance();
    //std::cout<<"detect Diagnoal"<<std::endl;
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    if (isCircle){
        const GLfloat a = abs(point[0]->getGeoCenter().x - geoCenter.x), b = abs(point[0]->getGeoCenter().y - geoCenter.y);
        const GLfloat xaxis = (cursorX - geoCenter.x) * (cursorX - geoCenter.x);
        const GLfloat yaxis = (cursorY - geoCenter.y) * (cursorY - geoCenter.y) ;
        if (isFill)
            return xaxis / (a * a) + yaxis / (b * b) <= 1;
        else{
            GLfloat currentLineWidth = lineWidth * Camera2D::getView().getZoom();
            bool outside = xaxis / ((a + currentLineWidth/2) * (a + currentLineWidth/2) )+ yaxis / ((b + currentLineWidth/2) * (b + lineWidth/2)) <=1;
            bool inside = xaxis / ((a - currentLineWidth/2) * (a - currentLineWidth/2) )+ yaxis / ((b - currentLineWidth/2) * (b - lineWidth/2)) >=1;
            return outside && inside;
        }
    }else{
        GLfloat minX,minY,maxX,maxY;
        if (point[0]->getGeoCenter().x > point[1]->getGeoCenter().x){
            minX = point[1]->getGeoCenter().x;
            maxX = point[0]->getGeoCenter().x;
        }
        else{
            minX = point[0]->getGeoCenter().x;
            maxX = point[1]->getGeoCenter().x;
        }
        if (point[0]->getGeoCenter().y > point[1]->getGeoCenter().y){
            minY = point[1]->getGeoCenter().y;
            maxY = point[0]->getGeoCenter().y;
        }
        else{
            minY = point[0]->getGeoCenter().y;
            maxY = point[1]->getGeoCenter().y;
        }
        if (isFill)
            return cursorX >= minX && cursorX <= maxX && cursorY >= minY && cursorY <= maxY;
        else{
            GLfloat currentLineWidth = lineWidth * Camera2D::getView().getZoom()* gui::detactBias;
            bool outside = cursorX >= (minX - currentLineWidth/2) && cursorX <= (maxX + currentLineWidth/2) && (cursorY >= minY-currentLineWidth/2) && (cursorY <= maxY + currentLineWidth/2);
            bool inside = cursorX >= (minX + currentLineWidth/2) && cursorX <= (maxX - currentLineWidth/2) && (cursorY >= minY + currentLineWidth/2) && (cursorY <= maxY - currentLineWidth/2);
            return outside && !inside;
        }
    }
    return false;
}
int outboundDetect(pElement outbound){
    return 12;
}
}//namespace pr

void createTopoElements(Primitive* lastpPrimitive){
    //const GLenum shape = lastpPrimitive->getShape();
    Take& take = Take::holdon();
    switch (take.drawType) {
        case Shape::POINTS:
            std::cout<<"treat as points"<<std::endl;
            for (int i = 0; i< lastpPrimitive->getVertexNum(); i++)
                lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Point>(lastpPrimitive,i)) );
            break;
        case Shape::LINES:
            std::cout<<"treat as line"<<std::endl;
            for (int i = 0; i< lastpPrimitive->getVertexNum()-1; i++)
                lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Line>(lastpPrimitive,i,i+1)) );
            break;
        case Shape::LOOP:
            std::cout<<"treat as face"<<std::endl;
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Face>(lastpPrimitive)) );
            break;
        case Shape::RECTANGLE:
            std::cout<<"treat as face"<<std::endl;
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Diagnoal>(lastpPrimitive,false)) );
            break;
        case Shape::POLYGEN:
            std::cout<<"treat as face"<<std::endl;
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Face>(lastpPrimitive)) );
            break;
        case Shape::CURVE:
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Curve>(lastpPrimitive)) );
            std::cout<<"treat as curve"<<std::endl;
            break;
        case Shape::CIRCLE:
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Diagnoal>(lastpPrimitive,true)) );
            std::cout<<"treat as circle"<<std::endl;
            break;
        default:
            std::cout<<"treat as none"<<std::endl;
            break;
    }
    return;
}

void updateTopoElements(Primitive* lastpPrimitive){
    ShaderStyle& style = ShaderStyle::getStyle();
    const ImVec4 nowColor = style.drawColor;
    const glm::vec4 primitiveColor = lastpPrimitive->getColor();
    lastpPrimitive->elementList.clear();
    style.drawColor = {primitiveColor.x,primitiveColor.y,primitiveColor.z,primitiveColor.w};
    const GLenum shape = lastpPrimitive->getShape();
    if (shape == GL_POINTS){
        for (int i = 0; i< lastpPrimitive->getVertexNum(); i++){
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Point>(lastpPrimitive,i,false)) );
            
        }
    }
    else if (shape == GL_LINES || shape == GL_LINE){
        for (int i = 0; i< lastpPrimitive->getVertexNum()-1; i++){
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Line>(lastpPrimitive,i,i+1,false)) );
        }
    }
    else{
        lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Face>(lastpPrimitive)) );
    }
    style.drawColor = nowColor;
    return;
}

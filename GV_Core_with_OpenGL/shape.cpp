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
    if (highlighted){
        ImVec4 backgroundColor = WindowParas::getInstance().backgroundColor;
        if (shape == GL_POINTS || shape == GL_POINT){
            glUniform4f(colorLoc,1-backgroundColor.x,1-backgroundColor.y,1-backgroundColor.z,1.0f);
        }else if (shape ==GL_LINES || shape == GL_LINE_LOOP){
            glUniform4f(colorLoc,1-backgroundColor.x - 0.2f,1-backgroundColor.y - 0.2f,1-backgroundColor.z - 0.2f,1.0f);
        }
        else{
            //glUnrangtaiform4f(colorLoc,style.color.x+ 0.1f,style.color.y+ 0.1f,style.color.z+0.1f,style.color.w);
            glUniform4f(colorLoc,style.color.x + 0.1f,style.color.y+ 0.1f,style.color.z + 0.1f,style.color.w);
        }
    }
    else{
        glUniform4f(colorLoc,style.color.x,style.color.y,style.color.z,style.color.w);
    }
}
void Point::draw(bool highlighted){
    setColor(highlighted);
    //thickness
    GLuint sizeLoc = glGetUniformLocation(shader->program,"thickness");
    glUniform1f(sizeLoc,pointSize);
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
    GLuint sizeLoc = glGetUniformLocation(shader->program,"thickness");
    glUniform1f(sizeLoc,lineWidth);
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
    glUniform1f(sizeLoc,1.0f);
    // load data
    glBindVertexArray(identifier->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(shape,static_cast<GLsizei>(vertexIndex.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return;
}
void Curve::draw(bool highlighted){
    
    return;
}
void OutBound::draw(bool highlighted){
    
    return;
}
void Dignoal::draw(bool highlighted){
    setColor(highlighted);
    //thickness
    GLuint sizeLoc = glGetUniformLocation(shader->program,"thickness");
    glUniform1f(sizeLoc,lineWidth);
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
    const GLfloat pointDetectRange = 5.0f;
    if ((cursorX - pointX) * (cursorX - pointX) + (cursorY - pointY) * (cursorY - pointY) <= pointDetectRange * pointDetectRange){
        return true;
    }
    return false;
}
bool Line::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    WindowParas& windowPara = WindowParas::getInstance();
    //std::cout<<"detect line"<<std::endl;
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    const GLfloat& pointX1 = (*refVertex)[vertexIndex[0] * stride],pointY1 = (*refVertex)[vertexIndex[0] * stride + 1];
    const GLfloat& pointX2 = (*refVertex)[vertexIndex[1] * stride],pointY2 = (*refVertex)[vertexIndex[1] * stride + 1];
    const GLfloat zoom = Camera2D::getView().getZoom();
    const GLfloat lineAngleRange = 0.2f * zoom, paralellRange = 5.0f * zoom;
    bool inTheRange = false,onTheSlop = false;
    if (abs(pointX2- pointX1)< paralellRange){
        inTheRange = (cursorY > pointY1) != (cursorY > pointY2);
        onTheSlop = (abs(pointX1 - cursorX)< paralellRange) || (abs(pointX2 - cursorX)< paralellRange);
    }else if (abs(pointY2 - pointY1)<paralellRange){
        inTheRange = (cursorX > pointX1) != (cursorX > pointX2);
        onTheSlop = (abs(pointY1 - cursorY)< paralellRange) || (abs(pointY2 - cursorY)< paralellRange);
    }else{
        inTheRange = ((cursorX > pointX1) != (cursorX > pointX2)) && (cursorY > pointY1) != (cursorY > pointY2);
        onTheSlop = abs((cursorX - pointX1)/ (cursorY - pointY1) - (pointX2 - pointX1)/ (pointY2 - pointY1)) <= lineAngleRange;
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
   
    return true;
}
bool Curve::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    
    return true;
}
bool Dignoal::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    
    return true;
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
        case Shape::TRIANGLE:
            std::cout<<"treat as face"<<std::endl;
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Face>(lastpPrimitive)) );
            break;
        case Shape::RECTANGLE:
            std::cout<<"treat as face"<<std::endl;
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Face>(lastpPrimitive)) );
            break;
        case Shape::POLYGEN:
            std::cout<<"treat as face"<<std::endl;
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Face>(lastpPrimitive)) );
            break;
        case Shape::CURVE:
            std::cout<<"treat as curve"<<std::endl;
            break;
        case Shape::CIRCLE:
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Dignoal>(lastpPrimitive)) );
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

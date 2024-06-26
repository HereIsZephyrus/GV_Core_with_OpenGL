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

void  Element::rend(GLuint& program) {
    //glUseProgram(program);
    //color
    GLuint colorLoc = glGetUniformLocation(program,"setColor");
    glUniform4f(colorLoc,style.color.x,style.color.y,style.color.z,style.color.w);
    //thickness
    //GLuint  resLoc  = glGetUniformLocation(program, "resolution");
    //GLuint  thiLoc  = glGetUniformLocation(program, "thickness");
    //glUniform1f(thiLoc,thickness);
    //WindowParas& windowPara = WindowParas::getInstance();
    //glUniform2f(resLoc, windowPara.SCREEN_WIDTH,windowPara.SCREEN_HEIGHT);
    
    //camera
    GLuint projectionLoc = glGetUniformLocation(program, "projection");
    GLuint viewLoc = glGetUniformLocation(program, "view");
    GLuint modelLoc = glGetUniformLocation(program, "model");
    Camera2D& camera = Camera2D::getView();
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}
void Element::load(){
    glBindVertexArray(identifier->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, identifier->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
}
void Element::draw(){
    //std::cout<<"Draw is running"<<std::endl;
    GLint currentBuffer;
    if (shader == nullptr){
        std::cerr<<"havn't bind shader";
        return;
    }
    else
        shader ->use();
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
    if (currentBuffer != EBO){
        //std::cout<<currentBuffer<<' '<<VBO<<std::endl;
        load();
    }
    glEnableVertexAttribArray(0);
    glBindVertexArray(identifier->VAO);;
    glDrawElements(shape,static_cast<GLsizei>(vertexIndex.size()), GL_UNSIGNED_INT, 0);
    //CHECK_GL_ERROR( glDrawElements(shape, indexLen, GL_UNSIGNED_INT, 0));
    rend(shader->program);
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
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    const GLfloat& pointX = (*refVertex)[vertexIndex[0] * stride],pointY = (*refVertex)[vertexIndex[0] * stride + 1];
    const GLfloat pointDetectRange = 3.0f;
    if ((cursorX - pointX) * (cursorX - pointX) + (cursorY - pointY) * (cursorY - pointY) <= pointDetectRange * pointDetectRange){
        return true;
    }
    return false;
}
bool Line::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    const GLfloat& pointX1 = (*refVertex)[vertexIndex[0] * stride],pointY1 = (*refVertex)[vertexIndex[0] * stride + 1];
    const GLfloat& pointX2 = (*refVertex)[vertexIndex[1] * stride],pointY2 = (*refVertex)[vertexIndex[1] * stride + 1];
    const GLfloat lineAngleRange = 1.0f;
    if (abs((cursorX - pointX1)/ (cursorY - pointY1) - (pointX2 - pointX1)/ (pointY2 - pointY1)) <= lineAngleRange){
        return true;
    }
    return false;
}
bool Face::cursorSelectDetect(GLdouble xpos,GLdouble ypos){
    WindowParas& windowPara = WindowParas::getInstance();
    const GLfloat cursorX = windowPara.normal2orthoX(windowPara.screen2normalX(xpos));
    const GLfloat cursorY = windowPara.normal2orthoY(windowPara.screen2normalY(ypos));
    bool inside = false;
    for (auto it = line.begin(); it != line.end(); it ++){
        const GLuint ind1 = (*it)->vertexIndex[0] ,ind2 = (*it)->vertexIndex[1];
        const GLfloat& pointX1 = (*refVertex)[ind1 * stride],pointY1 = (*refVertex)[ind1 * stride + 1];
        const GLfloat& pointX2 = (*refVertex)[ind2* stride],pointY2 = (*refVertex)[ind2* stride + 1];
        if ((pointY1 > cursorY) != (pointY2 > cursorY) && (cursorX < (pointX2 - pointX1) * (cursorY - pointY1) / (pointY2 - pointY1) + pointX1)){
            inside = !inside;
        }
    }
    return inside;
}
}//namespace pr

void createTopoElements(Primitive* lastpPrimitive){
    const GLenum shape = lastpPrimitive->getShape();
    if (shape == GL_POINTS){
        for (int i = 0; i< lastpPrimitive->getVertexNum(); i++)
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Point>(lastpPrimitive,i)) );
    }
    else if (shape == GL_LINES || shape == GL_LINE){
        for (int i = 0; i< lastpPrimitive->getVertexNum()-1; i++)
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Line>(lastpPrimitive,i,i+1)) );
    }
    else
        lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Face>(lastpPrimitive)) );
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
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Point>(lastpPrimitive,i)) );
            
        }
    }
    else if (shape == GL_LINES || shape == GL_LINE){
        for (int i = 0; i< lastpPrimitive->getVertexNum()-1; i++){
            lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Line>(lastpPrimitive,i,i+1)) );
        }
    }
    else{
        lastpPrimitive->elementList.push_back(std::static_pointer_cast<pr::Element>(std::make_shared<pr::Face>(lastpPrimitive)) );
    }
    style.drawColor = nowColor;
    return;
}

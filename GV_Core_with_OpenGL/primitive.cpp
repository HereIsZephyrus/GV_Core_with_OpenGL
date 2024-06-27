//
//  primitive.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/9/24.
//

#include <iostream>
#include <cmath>
#include "primitive.hpp"
#include "glexception.hpp"
#include "commander.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "rendering.hpp"
Primitive::Primitive(vertexArray vertices,Shape shape,GLsizei stride):stride(stride){
    if (!HAS_INIT_OPENGL_CONTEXT)
        initOpenGL(WindowParas::getInstance().window);
    m_self = this;
    this->vertices = vertices;
    this->elementList.clear();
    this->holding = false;
    //this->transMat = glm::mat4(1.0f);
    Records& record = Records::getState();
    this->layer = static_cast<GLuint>(record.primitiveList.size())+1;
    const ImVec4 uiColor = ShaderStyle::getStyle().drawColor;
    this->color = {uiColor.x,uiColor.y,uiColor.z,uiColor.w};
    switch (shape) {
        case Shape::POINTS:{
            this->shape = GL_POINTS;
            break;
        }
        case Shape::LINES:{
            this->shape = GL_LINES;
            break;
        }
        case Shape::TRIANGLE:{
            this->shape = GL_TRIANGLES;
            break;
        }
        case Shape::RECTANGLE:{
            this->shape = GL_TRIANGLE_FAN;
            break;
        }
        case Shape::CIRCLE:{ //not achieved
            this->shape = GL_TRIANGLE_STRIP;
            //this->indices = pr
            break;
        }
        case Shape::LOOP:{
            this->shape = GL_LINE_LOOP;
            break;
        }
        case Shape::POLYGEN:{
            if (ShaderStyle::getStyle().toFill)
                this->shape = GL_TRIANGLE_FAN;
            else
                this->shape = GL_LINE_LOOP;
            break;;
        }
        case Shape::CURVE:{
            this->shape = GL_LINE_STRIP;
            generateCurve();
            break;
        }
        default:{
            this->shape = GL_POINT;
            break;
        }
    }
    shader = nullptr;
    glGenVertexArrays(1,&identifier.VAO);
    glGenBuffers(1,&identifier.VBO);
    updateVertex();
}
void Primitive::rend(GLuint& program){
    const glm::vec4 color = previewStyle::color;
    GLuint colorLoc = glGetUniformLocation(program,"setColor");
    glUniform4f(colorLoc,color.x,color.y,color.z,color.w);
    
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
void Primitive::load(){
    glBindVertexArray(identifier.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, identifier.VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
}
void Primitive::draw(){
    //std::cout<<"Draw is running"<<std::endl;
    GLint currentBuffer;
    if (shader == nullptr){
        std::cerr<<"havn't bind shader"<<std::endl;
        return;
    }
    else
        shader ->use();
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
    if (currentBuffer != identifier.VBO){
        //std::cout<<currentBuffer<<' '<<VBO<<std::endl;
        load();
    }
    glEnableVertexAttribArray(0);
    glBindVertexArray(identifier.VAO);
    //std::cout<<"drawing primitive"<<std::endl;
    rend(shader->program);
    //glDrawArrays(shape, 0,( getVertexNum()-1)*6);
    glDrawArrays(shape, 0, getVertexNum());
    // CHECK_GL_ERROR(glDrawArrays(shape, 0, vertexCount));
    glBindVertexArray(0);
    return;
}
 
void Primitive::updateVertex(){
    glBindVertexArray(identifier.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, identifier.VBO);
    //glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(transfered.size() * sizeof(GLfloat)) ,static_cast<const void*>(transfered.data()), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(GLfloat)) ,static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Primitive::transform(const glm::mat3& inputMat){
    //transfered = vertices;
    for (auto vertex = vertices.begin(); vertex != vertices.end(); vertex+=stride) {
        const GLfloat rawX = *(vertex), rawY = *(vertex+1);
        //std::cout<< inputMat[0][0]<<' '<< inputMat[1][0]<< inputMat[2][0]<<std::endl;
        //std::cout<< inputMat[0][1]<<' '<< inputMat[1][1]<< inputMat[2][1]<<std::endl;
        *(vertex) = rawX * inputMat[0][0] + rawY * inputMat[1][0] + inputMat[2][0];
        *(vertex+1) = rawX * inputMat[0][1] + rawY * inputMat[1][1] + inputMat[2][1];
    }
    updateVertex();
}
void Primitive::transform(const indexArray& vertexIndex,const glm::mat3& inputMat){
    //transfered = vertices;
    for (auto index = vertexIndex.begin(); index != vertexIndex.end(); index++) {
        const GLint beginIndex = (*index) * stride;
        const GLfloat rawX = vertices[beginIndex], rawY = vertices[beginIndex + 1];
        vertices[beginIndex] = rawX * inputMat[0][0] + rawY * inputMat[1][0] + inputMat[2][0];
        vertices[beginIndex+1] = rawX * inputMat[0][1] + rawY * inputMat[1][1] + inputMat[2][1];
    }
}
void Primitive::createOutboundElement(){
    
}
void Primitive::destroyOutboundElement(){
    
}
GLfloat lagrange_basis(int i, GLfloat x, const std::vector<GLfloat>& points) {
    GLfloat basis = 1.0f;
    for (size_t j = 0; j < points.size(); ++j) {
        if (j != i)
            basis *= (x - points[j]) / (points[i] - points[j]);
    }
    return basis;
}
GLfloat Primitive::lagrangeInterpolation(GLfloat x,const vertexArray& controlArray,const GLsizei numControlPoints){
    GLfloat y = 0.0f;
    std::vector<GLfloat> xVal;
    for (auto vertex = controlArray.begin(); vertex!=controlArray.end(); vertex+=stride)
        xVal.push_back(*vertex);
        
    for (int i = 0; i < numControlPoints; i++) {
            GLfloat basis = lagrange_basis(i, x, xVal);
            y +=controlArray[i * stride + 1] * basis;
        }
    return y;
}
void Primitive::generateCurve(){
    vertexArray controlArray = vertices;
    const GLsizei numControlPoints = getVertexNum();
    vertices.clear();
    GLfloat length = 0;
    for (int i = 1; i < numControlPoints; i++) {
        length += std::sqrt(
                        (controlArray[i * stride] - controlArray[(i-1) * stride]) * (controlArray[i * stride] - controlArray[(i-1) * stride])+
                        (controlArray[i * stride + 1] - controlArray[(i-1) * stride + 1]) * (controlArray[i * stride + 1] - controlArray[(i-1) * stride] + 1)+
                        (controlArray[i * stride + 2] - controlArray[(i-1) * stride + 2]) * (controlArray[i * stride + 2] - controlArray[(i-1) * stride] + 2)
                        );
    }
    const int numInterpolated = static_cast<int>(length);
    //std::cout<<"curve point num:"<<numInterpolated<<std::endl;
    const GLfloat startX = controlArray[0],endX = controlArray[(numControlPoints-1) * stride];
    for (int i = 0; i < numInterpolated; ++i) {
        GLfloat x = startX + i * (endX - startX) / (numInterpolated - 1);
        GLfloat y = lagrangeInterpolation(x, controlArray,numControlPoints);
        vertices.push_back(x);vertices.push_back(y);vertices.push_back(0);
    }
}
namespace pr {
std::vector<std::unique_ptr<Primitive> >mainPrimitiveList;
pPrimitive drawPreviewPrimitive = nullptr;
pPrimitive axisPrimitive = nullptr;
}

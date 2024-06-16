//
//  primitive.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/9/24.
//

#include <iostream>
#include "primitive.hpp"
#include "glexception.hpp"
#include "commander.hpp"
#include "window.hpp"
#include "camera.hpp"
Primitive::Primitive(vertexArray vertices,Shape shape,GLsizei stride):stride(stride){
    if (!HAS_INIT_OPENGL_CONTEXT)
        initOpenGL(WindowParas::getInstance().window);
    this->vertices = vertices;
    switch (shape) {
        case Shape::POINTS:{
            this->shape = GL_POINTS;
            this->type = DrawType::Array;
            break;
        }
        case Shape::LINES:{
            this->shape = GL_LINES;
            this->type = DrawType::Array;
            break;
        }
        case Shape::TRIANGLE:{
            this->shape = GL_TRIANGLES;
            this->type = DrawType::Array;
            break;
        }
        case Shape::RECTANGLE:{
            this->shape = GL_TRIANGLE_FAN;
            this->type = DrawType::Array;
            break;
        }
        case Shape::CIRCLE:{ //not achieved
            this->shape = GL_TRIANGLE_STRIP;
            this->type = DrawType::Index;
            //this->indices = pr
            break;
        }
        case Shape::LOOP:{
            this->shape = GL_LINE_LOOP;
            this->type = DrawType::Array;
            break;
        }
        case Shape::POLYGEN:{
            if (ShaderStyle::getStyle().toFill)
                this->shape = GL_TRIANGLE_FAN;
            else
                this->shape = GL_LINE_LOOP;
            this->type = DrawType::Array;
            break;;
        }
        default:{
            this->shape = GL_POINT;
            this->type = DrawType::Array;
            break;
        }
    }
    shader = nullptr;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(GLfloat)) ,static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
/*
Primitive::Primitive(vertexArray vertices,indexArray indices,Shape shape,GLsizei stride,GLsizei indlen):
type{DrawType::Index},shape(shape),indexLen(indlen),stride(stride){
    if (!HAS_INIT_OPENGL_CONTEXT)
        initOpenGL(WindowParas::getInstance().window);
    this->vertices = vertices;
    this->indices = indices;
    shader = nullptr;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(GLfloat)) ,static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,  static_cast<GLsizei>(indices.size() * sizeof(GLuint)), static_cast<const void*>(indices.data()), GL_STATIC_DRAW);
}
 */
void Primitive::load(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (type == DrawType::Index)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
}
static void bindCameraProjection(Shader* shader){
    Camera2D& camera = Camera2D::getView();
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    GLuint projectionLoc = glGetUniformLocation(shader->getProgram(), "projection");
    GLuint viewLoc = glGetUniformLocation(shader->getProgram(), "view");
    GLuint modelLoc = glGetUniformLocation(shader->getProgram(), "model");
    
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    return;
}
void Primitive::draw(){
    //std::cout<<"Draw is running"<<std::endl;
    GLint currentBuffer;
    if (shader == nullptr){
        std::cerr<<"havn't bind shader";
        return;
    }
    else{
        bindCameraProjection(shader);
        shader ->rend();
    }
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
    if (currentBuffer != VBO){
        //std::cout<<currentBuffer<<' '<<VBO<<std::endl;
        load();
    }
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO);
    //std::cout<<VAO<<std::endl;
    if (type == DrawType::Array){
        glDrawArrays(shape, 0, getVertexNum());
       // CHECK_GL_ERROR(glDrawArrays(shape, 0, vertexCount));
    }
    if (type == DrawType::Index){
        glDrawElements(shape, indexLen, GL_UNSIGNED_INT, 0);
        //CHECK_GL_ERROR( glDrawElements(shape, indexLen, GL_UNSIGNED_INT, 0));
    }
    glBindVertexArray(0);
    return;
}

namespace pr {
std::vector<std::unique_ptr<Primitive> >mainPrimitiveList;
pPrimitive drawPreviewPrimitive = nullptr;
static const indexArray rectIndex ={
    0, 1, 3,//right-top,right-button,left-top
    1, 2, 3//right-button,left-button,left-top
};
}

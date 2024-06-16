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
        default:{
            this->shape = GL_POINT;
            break;
        }
    }
    shader = nullptr;
    updateVertex();
}
void Primitive::load(){
    glBindVertexArray(identifier.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, identifier.VBO);
    if (type == DrawType::Index)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier.EBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
}
void Primitive::draw(){
    //std::cout<<"Draw is running"<<std::endl;
    GLint currentBuffer;
    if (shader == nullptr){
        std::cerr<<"havn't bind shader";
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
    //std::cout<<VAO<<std::endl;
    if (type == DrawType::Array){
        //glDrawArrays(shape, 0,( getVertexNum()-1)*6);
        glDrawArrays(shape, 0, getVertexNum());
       // CHECK_GL_ERROR(glDrawArrays(shape, 0, vertexCount));
    }
    if (type == DrawType::Index){
        glDrawElements(shape, indexLen, GL_UNSIGNED_INT, 0);
        //CHECK_GL_ERROR( glDrawElements(shape, indexLen, GL_UNSIGNED_INT, 0));
    }
    shader->rend();
    glBindVertexArray(0);
    return;
}
void Primitive::updateVertex(){
    glGenVertexArrays(1,&identifier.VAO);
    glBindVertexArray(identifier.VAO);
    glGenBuffers(1,&identifier.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, identifier.VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(GLfloat)) ,static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
    if (!indices.empty()){
        glGenBuffers(1, &identifier.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,  static_cast<GLsizei>(indices.size() * sizeof(GLuint)), static_cast<const void*>(indices.data()), GL_STATIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

namespace pr {
std::vector<std::unique_ptr<Primitive> >mainPrimitiveList;
pPrimitive drawPreviewPrimitive = nullptr;
}

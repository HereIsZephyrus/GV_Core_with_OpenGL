//
//  primitive.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/9/24.
//

#include <iostream>
#include "primitive.hpp"
#include "glexception.hpp"
Primitive::Primitive(vertexArray vertices,GLenum shape,GLsizei stride):type{DrawType::Array},shape(shape),indexLen(0),stride(stride){
    this->vertices = vertices;
    this->indices = {};
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
}
Primitive::Primitive(vertexArray vertices,indexArray indices,GLenum shape,GLsizei stride,GLsizei indlen):type{DrawType::Index},shape(shape),indexLen(indlen),stride(stride){
    this->vertices = vertices;
    this->indices = indices;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1, &EBO);
}
void Primitive::load(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    const void* dataHead = static_cast<const void*>(vertices.data());
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(GLfloat)) ,dataHead, GL_STATIC_DRAW);
    //std::cout<<*static_cast<const GLfloat*>(dataHead)<<std::endl;
    if (type == DrawType::Index){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,  static_cast<GLsizei>(indices.size() * sizeof(GLuint)), static_cast<const void*>(indices.data()), GL_STATIC_DRAW);
    }
    //glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride * sizeof (GLfloat),(GLvoid *)0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
}
void Primitive::draw(){
    //std::cout<<"Draw is running"<<std::endl;
    GLint currentBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
    if (currentBuffer != VBO){
        //std::cout<<currentBuffer<<' '<<VBO<<std::endl;
        load();
    }
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO);
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

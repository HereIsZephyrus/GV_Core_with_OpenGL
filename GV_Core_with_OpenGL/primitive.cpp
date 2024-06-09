//
//  primitive.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/9/24.
//

#include "primitive.hpp"
Primitive::Primitive(GLfloat* vertices,GLenum shape,GLsizei count,GLsizei stride):type{DrawType::Array},shape(shape){
    //init memoery
    this->vertices = new GLfloat[count*stride];
    indices = nullptr;
    std::memcpy(this->vertices, vertices, sizeof(GLfloat)*stride*count);
    //generate index
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
}
Primitive::Primitive(GLfloat* vertices,GLuint* indices,GLenum shape,GLsizei count,GLsizei stride,GLsizei indlen):type{DrawType::Index},shape(shape),indexLen(indlen){
    //init memoery
    this->vertices = new GLfloat[count*stride];
    this->indices = new GLuint[indexLen];
    std::memcpy(this->vertices, vertices, sizeof(GLfloat)*stride*count);
    std::memcpy(this->indices, indices, sizeof(GLuint) *indexLen);
    //generate index
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1, &EBO);
}
void Primitive::draw(){
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
    if (type == DrawType::Array){
        glBindVertexArray(VAO);
        glDrawArrays(shape, 0, vertexCount);
        glBindVertexArray(0);
    }
    if (type == DrawType::Index){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindVertexArray(VAO);
        glDrawElements(shape, indexLen, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    return;
}

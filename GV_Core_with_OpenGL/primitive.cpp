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
Primitive::Primitive(vertexArray vertices,GLenum shape,GLsizei stride):
type{DrawType::Array},shape(shape),indexLen(0),stride(stride){
    if (!HAS_INIT_OPENGL_CONTEXT)
        initOpenGL(WindowParas::getInstance().window);
    this->vertices = vertices;
    this->indices = {};
    shader = nullptr;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(GLfloat)) ,static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
}
Primitive::Primitive(vertexArray vertices,indexArray indices,GLenum shape,GLsizei stride,GLsizei indlen):
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
std::vector<std::unique_ptr<Primitive> >primitives;
pPrimitive drawPreviewPrimitive = nullptr;
void createPrimitiveTo(std::vector<pPrimitive >& primitiveList){
    Take& take = Take::holdon();
    pPrimitive newPrimitive (new Primitive(take.drawingVertices, take.drawType, 3));
    newPrimitive->bindShader(rd::defaultShader);
    pr::primitives.push_back(std::move(newPrimitive));
    return;
}
static const vertexArray tranVertex = {
    -250.0f, -250.0f, 0.0f,
    250.0f, -250.0f, 0.0f,
    0.0f,  250.0f, 0.0f
};
static const indexArray indices ={
    0, 1, 3,
    1, 2, 3
};
static const vertexArray rectVertex ={
    200.0f, 200.0f, 0.0f,
    200.0f, -200.0f, 0.0f,
    -200.0f, -200.0f, 0.0f,
     -200.0f, 200.0f, 0.0f
};
std::unique_ptr<Primitive> rectangle(new Primitive(rectVertex, indices, GL_TRIANGLES, 4,  6));
std::unique_ptr<Primitive> triangle(new Primitive(tranVertex, GL_TRIANGLES,  3));
}

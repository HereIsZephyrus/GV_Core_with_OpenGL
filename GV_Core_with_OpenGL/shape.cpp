//
//  shape.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/16/24.
//

#include "shape.hpp"
namespace pr{

GLuint elementNum = 0;
std::vector<pElement > elements;

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
    if (currentBuffer != identifier->VBO){
        //std::cout<<currentBuffer<<' '<<VBO<<std::endl;
        load();
    }
    glEnableVertexAttribArray(0);
    glBindVertexArray(identifier->VAO);
    //std::cout<<VAO<<std::endl;
    glDrawElements(shape,static_cast<GLsizei>(vertexIndex.size()), GL_UNSIGNED_INT, 0);
    //CHECK_GL_ERROR( glDrawElements(shape, indexLen, GL_UNSIGNED_INT, 0));
    shader->rend();
    glBindVertexArray(0);
    return;
}

}

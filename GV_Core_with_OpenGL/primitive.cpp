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
static void DDA(vertexArray& array,float sX,float sY,float tX,float tY){
    float dx = tX - sX,dy = tY - sY;
    int steps = 0;
    if (std::abs(dx) >= std::abs(dy)){
        steps = std::abs(dx);
    }
    else{
        steps = std::abs(dy);
    }
    float xinc = dx / steps;
    float yinc = dy / steps;
    float x = sX,y = sY;
    //std::cout<<x<<y<<std::endl;

    for (int i = 0; i <= steps; i++) {
        x += xinc;
        y += yinc;
        array.push_back(x);
        array.push_back(y);
        array.push_back(0.0f);
    }
}
static void drawLine(vertexArray& array) {
    const GLfloat thickness = ShaderStyle::getStyle().thickness;
    const GLfloat sX = array[0],sY = array[1], tX = array[3],tY = array[4];
    std::cout<<sX << ' '<<sY<<' '<<tX << ' '<<tY<<std::endl;
    const int range = (thickness + 0.5)/2;
    for (int i = -range; i<= range; i++)
        for (int j = -range; j<=range; j++)
            DDA(array,sX+i,sY+j,tX+i,tY+j);
}
Primitive::Primitive(vertexArray vertices,Shape shape,GLsizei stride):stride(stride){
    if (!HAS_INIT_OPENGL_CONTEXT)
        initOpenGL(WindowParas::getInstance().window);
    this->vertices = vertices;
    this->indices.clear();
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
    //if (this->shape == GL_LINES){
    //    std::cout<<"it is working"<<vertices.size()<<std::endl;
    //    for (auto it = vertices.begin(); it!=vertices.end(); it++)
    //        std::cout<<*it<<' ';
    //    std::cout<<std::endl;
    //    drawLine(this->vertices);
    //}
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
void Primitive::load(){
    glBindVertexArray(identifier.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, identifier.VBO);
    if (type == DrawType::Index)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier.EBO);
    if (stride == 8){
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // 颜色属性
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(4* sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }
    else if (stride ==3){
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride * sizeof (GLfloat),(GLvoid *)0);
        glEnableVertexAttribArray(0);
    }
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
        //glDrawArrays(shape, 0, getVertexNum());
        if (shape == GL_LINES)
            glDrawArrays(GL_POINTS, 0, getVertexNum());//manually draw
        else
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

namespace pr {
std::vector<std::unique_ptr<Primitive> >mainPrimitiveList;
pPrimitive drawPreviewPrimitive = nullptr;
static const indexArray rectIndex ={
    0, 1, 3,//right-top,right-button,left-top
    1, 2, 3//right-button,left-button,left-top
};
}

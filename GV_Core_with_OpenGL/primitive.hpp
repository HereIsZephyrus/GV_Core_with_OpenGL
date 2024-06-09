//
//  primitive.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/9/24.
//

#ifndef primitive_hpp
#define primitive_hpp

#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

enum class DrawType{
    Array,
    Index,
};
class Primitive{
public:
    Primitive(GLfloat* vertices,GLenum shape,GLsizei count,GLsizei stride);
    Primitive(GLfloat* vertices,GLuint* indices,GLenum shape,GLsizei count,GLsizei stride,GLsizei indlen);
    ~Primitive(){
        delete [] vertices;
        delete [] indices;
        glDeleteVertexArrays(1,&VAO);
        glDeleteBuffers(1,&VBO);
        if (type == DrawType::Index)
            glDeleteBuffers(1,&EBO);
    }
    void draw();
private:
    GLuint VAO,VBO,EBO;
    GLenum shape;
    GLsizei vertexCount,stride,indexLen;
    DrawType type;
    GLfloat* vertices;
    GLuint* indices;
};

namespace pr {
inline  GLfloat vertices[] ={
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
     -0.5f, 0.5f, 0.0f
};
inline  GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};
extern std::vector<Primitive> primitives;
}

#endif /* primitive_hpp */

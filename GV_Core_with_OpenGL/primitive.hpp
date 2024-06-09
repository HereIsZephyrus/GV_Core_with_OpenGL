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

typedef std::vector<GLfloat> vertexArray;
typedef std::vector<GLuint> indexArray;
enum class DrawType{
    Array,
    Index,
};
class Primitive{
public:
    Primitive(vertexArray vertices,GLenum shape,GLsizei stride);
    Primitive(vertexArray vertices,indexArray indices,GLenum shape,GLsizei stride,GLsizei indlen);
    ~Primitive(){
        vertices.clear();
        indices.clear();
        glDeleteVertexArrays(1,&VAO);
        glDeleteBuffers(1,&VBO);
        if (type == DrawType::Index)
            glDeleteBuffers(1,&EBO);
    }
    void draw();
    void load();
private:
    GLuint VAO,VBO,EBO;
    GLenum shape;
    GLsizei stride,indexLen;
    DrawType type;
    vertexArray vertices;
    indexArray indices;
    inline const GLsizei getVertexNum(){
        return static_cast<GLsizei>(vertices.size() / stride);
    }
};

namespace pr {

inline vertexArray tranVertex = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};
inline indexArray indices ={
    0, 1, 3,
    1, 2, 3
};
inline vertexArray rectVertex ={
    0.4f, 0.4f, 0.0f,
    0.4f, -0.4f, 0.0f,
    -0.4f, -0.4f, 0.0f,
     -0.4f, 0.4f, 0.0f
};
extern std::vector<Primitive> primitives;
}

#endif /* primitive_hpp */

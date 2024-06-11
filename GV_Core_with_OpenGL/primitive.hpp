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
#include <memory>
#include <vector>
#include "rendering.hpp"

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
    //Primitive(){}
    Primitive(const Primitive&) = delete;
    void operator=(const Primitive&) = delete;
    //Shader* shader;
    ~Primitive(){
        vertices.clear();
        indices.clear();
        glDeleteVertexArrays(1,&VAO);
        glDeleteBuffers(1,&VBO);
        if (type == DrawType::Index)
            glDeleteBuffers(1,&EBO);
    }
    void bindShader(const Shader* tobind){shader = tobind;}
    void draw();
    void load();
private:
    GLuint VAO,VBO,EBO;
    GLenum shape;
    GLsizei stride,indexLen;
    DrawType type;
    vertexArray vertices;
    indexArray indices;
    const Shader* shader;
    inline const GLsizei getVertexNum(){
        return static_cast<GLsizei>(vertices.size() / stride);
    }
};
typedef std::unique_ptr<Primitive> pPrimitive;
namespace pr {
extern pPrimitive rectangle;
extern pPrimitive triangle;
extern std::vector<pPrimitive >primitives;
}

#endif /* primitive_hpp */

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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>
#include "rendering.hpp"

typedef std::vector<GLfloat> vertexArray;
typedef std::vector<GLuint> indexArray;
enum class Shape{
    NONE,
    POINTS,
    LINES,
    TRIANGLE,
    STRIPE,
    RECTANGLE,
    CIRCLE,
    LOOP,
    POLYGEN
};
struct primitiveIdentifier{
    GLuint VAO,VBO;
};
namespace pr {
class Point;
class Line;
class Face;
class Element;
}
class Primitive{
public:
    Primitive(vertexArray vertices,Shape shape,GLsizei stride);
    Primitive(const Primitive&) = delete;
    void operator=(const Primitive&) = delete;
    //Shader* shader;
    ~Primitive(){
        vertices.clear();
        glDeleteVertexArrays(1,&identifier.VAO);
        glDeleteBuffers(1,&identifier.VBO);
    }
    void bindShader(Shader* tobind){shader = tobind;}
    
    const primitiveIdentifier* getIdentifier() const{return &identifier;}
    friend class pr::Element;
    friend class pr::Point;
    friend class pr::Line;
    friend class pr::Face;
private:
    primitiveIdentifier identifier;
    GLenum shape;
    GLsizei stride,indexLen;
    vertexArray vertices;
    Shader* shader;
    inline const GLsizei getVertexNum(){
        return static_cast<GLsizei>(vertices.size() / stride);
    }
};
typedef std::unique_ptr<Primitive> pPrimitive;

namespace pr {
extern pPrimitive drawPreviewPrimitive;
extern std::vector<pPrimitive >mainPrimitiveList;
}

#endif /* primitive_hpp */

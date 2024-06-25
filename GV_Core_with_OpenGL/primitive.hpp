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
typedef std::shared_ptr<pr::Element> pElement;
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
    void updateVertex();
    void load();
    void draw();
    void drawElement();
    void rend(GLuint& program);
    const primitiveIdentifier* getIdentifier() const{return &identifier;}
    glm::vec4 getColor() const{return color;}
    GLenum getShape() const{return shape;}
    friend class pr::Element;
    friend class pr::Point;
    friend class pr::Line;
    friend class pr::Face;
    friend void clipByShape();
    GLsizei getVertexNum() const{return static_cast<GLsizei>(vertices.size() / stride);}
    void setName(std::string name){this->name = name;}
    std::string getName() const {return name;}
    vertexArray vertices;
    std::vector<pElement> elementList;
    GLuint layer;
    bool operator < (const Primitive& x) const{
        return layer<x.layer;
    }
    Primitive* getSelf() {return m_self;}
private:
    primitiveIdentifier identifier;
    GLenum shape;
    GLsizei stride,indexLen;
    Shader* shader;
    glm::vec4 color;
    std::string name;
    Primitive* m_self;
};

typedef std::unique_ptr<Primitive> pPrimitive;
namespace pr {
extern pPrimitive drawPreviewPrimitive;
extern pPrimitive axisPrimitive;
extern std::vector<pPrimitive >mainPrimitiveList;
}

#endif /* primitive_hpp */

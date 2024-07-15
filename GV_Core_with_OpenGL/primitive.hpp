//
//  primitive.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/9/24.
//

#ifndef primitive_hpp
#define primitive_hpp

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
    RECTANGLE,
    CIRCLE,
    LOOP,
    POLYGEN,
    CURVE,
    MARKER
};
struct primitiveIdentifier{
    GLuint VAO,VBO;
};
namespace pr {
class Element;
class Point;
class Line;
class Face;
class Curve;
class Dignoal;
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
    void draw();
    void transformVertex(const indexArray&,const glm::mat3&);
    //void transform(const glm::mat3&);
    void drawElement();
    void rend(GLuint& program);
    const primitiveIdentifier* getIdentifier() const{return &identifier;}
    glm::vec4 getColor() const{return color;}
    GLenum getShape() const{return shape;}
    friend class pr::Element;
    friend class pr::Point;
    friend class pr::Line;
    friend class pr::Face;
    friend class pr::Curve;
    friend class pr::Dignoal;
    friend void clipByShape();
    GLsizei getVertexNum() const{return static_cast<GLsizei>(vertices.size() / stride);}
    void setName(std::string name){this->name = name;}
    void setHold(bool isHolding){this->holding = isHolding;}
    bool getHold() const{return holding;}
    std::string getName() const {return name;}
    vertexArray vertices;//,transfered;
    std::vector<pElement> elementList;
    GLuint layer;
    bool operator < (const Primitive& x) const{
        return layer<x.layer;
    }
    void createOutboundElement();
    void destroyOutboundElement();
    Primitive* getSelf() {return m_self;}
    void useShader();
    glm::mat3 transMat;
    //void addMat(const glm::mat3& inputMat){transMat = transMat * inputMat;}
protected:
    void generateCurve();
    void lagrangeInterpolation(const GLint,const vertexArray& ,const GLsizei);
    void splineInterpolation(const GLint,const vertexArray& ,const GLsizei);
    void bezierInterpolation(const GLint,const vertexArray& ,const GLsizei);
    void hermiteInterpolation(const GLint,const vertexArray& ,const GLsizei);
private:
    primitiveIdentifier identifier;
    GLenum shape;
    GLsizei stride,indexLen;
    Shader* shader;
    glm::vec4 color;
    std::string name;
    Primitive* m_self;
    bool holding;
    GLfloat thickness,pointsize;
};

typedef std::unique_ptr<Primitive> pPrimitive;
namespace pr {
extern pPrimitive previewPrimitive;
extern pPrimitive axisPrimitive;
extern std::vector<pPrimitive >mainPrimitiveList;
struct spline{
    GLfloat a,b,c,d;
};
}

#endif /* primitive_hpp */

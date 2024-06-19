//
//  shape.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/16/24.
//

#ifndef shape_hpp
#define shape_hpp

#include <iostream>
#include <memory>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "primitive.hpp"
#include "rendering.hpp"

typedef std::shared_ptr<vertexArray> pVertexArray;
void createTopoElements(const Primitive* lastpPrimitive);
void updateTopoElements(const Primitive* lastpPrimitive);

namespace pr {
void updateIndex(const Primitive*);
enum class TopoType{
    point,
    line,
    face
};
//don't recycle point/line/face index -- don't need to tackle so much elements for now.
extern GLuint elementNum;
class Element{
public:
    void draw();
    void load();
    void rend(GLuint& program) ;
    Element(const Primitive* primitive){
        refVertex = std::make_shared<vertexArray>(primitive->vertices);
        identifier = primitive->getIdentifier();
        shader = primitive->shader;
        const ImVec4 uiColor = ShaderStyle::getStyle().drawColor;
        style.color = {uiColor.x,uiColor.y,uiColor.z,uiColor.w};
        visable = true;
    }
    bool getVisable() const {return visable;}
    TopoType getType() const {return type;}
    const primitiveIdentifier* getIdentifier() const{return identifier;}
    void setColor(const glm::vec4 setColor){style.color = {setColor.x,setColor.y,setColor.z,setColor.w};}
    friend void createTopoElements(const Primitive* lastpPrimitive);
protected:
    void bindEBObuffer(){
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,  static_cast<GLsizei>(vertexIndex.size() * sizeof(GLuint)), static_cast<const void*>(vertexIndex.data()), GL_STATIC_DRAW);
    }
    virtual void calcGeoCenter()=0;
    indexArray vertexIndex;
    pVertexArray refVertex;
    glm::vec2 centerLocation;
    bool visable;
    GLuint EBO;
    const primitiveIdentifier* identifier;
    Shader* shader;
    GLenum shape;
    struct Style{
        glm::vec4 color;
    } style;
    TopoType type;
    //ShaderPara style;
};
typedef std::shared_ptr<Element> pElement;
extern std::vector<pElement > mainElementList;

class Point: public Element{
public:
    Point(const Primitive* primitive,GLuint startIndex):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        this->pointSize = style.pointSize;
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = GL_POINTS;
        vertexIndex = {startIndex};
        calcGeoCenter();
        type = TopoType::point;
        id  = ++elementNum;
        bindEBObuffer();
    }
    glm::vec2 getCenterLocation() const{return centerLocation;}
    
protected:
    void calcGeoCenter(){
        centerLocation.x = (*refVertex)[0];
        centerLocation.y = (*refVertex)[1];
    }
    GLuint id;
private:
    GLfloat pointSize;
};
class Line: public Element{
public:
    Line(const Primitive* primitive,GLuint startIndex,GLuint endIndex):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        this->lineWidth = style.thickness;
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = GL_LINES;
        vertexIndex = {startIndex,endIndex};
        point[0] = std::make_shared<Point>(primitive,vertexIndex[0]);
        point[1] = std::make_shared<Point>(primitive,vertexIndex[1]);
        mainElementList.push_back(point[0]);
        mainElementList.push_back(point[1]);
        calcGeoCenter();
        type = TopoType::line;
        id = ++ elementNum;
        bindEBObuffer();
    }
    glm::vec2 getCenterLocation() const{return centerLocation;}
protected:
    void calcGeoCenter(){
        centerLocation.x = 0;
        centerLocation.y = 0;
        glm::vec2 centerLoc1 = (*point[0]).getCenterLocation(),centerLoc2 = (*point[1]).getCenterLocation();
        centerLocation.x = (centerLoc1.x + centerLoc2.x)/2;
        centerLocation.y = (centerLoc1.y + centerLoc2.y)/2;
    }
    GLuint id;
private:
    GLfloat lineWidth;
    std::shared_ptr<Point> point[2];
};
typedef std::shared_ptr<Line> pLine;
class Face: public Element{
public:
    Face(const Primitive* primitive):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = primitive->shape;
        const GLsizei stride = primitive->stride;
        const int n =  static_cast<int>((*refVertex).size()/stride);
        for (int i = 0; i<n-1; i++){
            vertexIndex.push_back(i);
            line.push_back(std::make_shared<Line>(primitive,i,i+1));
            mainElementList.push_back(line.back());
        }
        vertexIndex.push_back(n-1);
        line.push_back(std::make_shared<Line>(primitive,n-1,0));
        mainElementList.push_back(line.back());
        calcGeoCenter();
        type = TopoType::face;
        id = ++ elementNum;
        bindEBObuffer();
    }
protected:
    void calcGeoCenter(){
        centerLocation.x = 0;
        centerLocation.y = 0;
        const int vertexNum = static_cast<int>(vertexIndex.size());
        for (auto it = line.begin(); it!=line.end(); it++){
            const glm::vec2 location = (*(*it)).getCenterLocation();
            centerLocation.x += location.x;
            centerLocation.y += location.y;
        }
        centerLocation.x /= vertexNum;
        centerLocation.y /= vertexNum;
    }
    GLuint id;
private:
    std::vector<pLine> line;
};
}
#endif /* shape_hpp */

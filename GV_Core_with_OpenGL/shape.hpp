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
void createTopoElements(Primitive* lastpPrimitive);
void updateTopoElements(Primitive* lastpPrimitive);

namespace pr {
void updateIndex(Primitive*);
enum class TopoType{
    point,
    line,
    face
};
class Line;
class Face;
//don't recycle point/line/face index -- don't need to tackle so much elements for now.
class Element{
public:
    virtual void draw(bool highlighted)=0;
    Element(const Primitive* primitive){
        refVertex = std::make_shared<vertexArray>(primitive->vertices);
        identifier = primitive->getIdentifier();
        shader = primitive->shader;
        const ImVec4 uiColor = ShaderStyle::getStyle().drawColor;
        style.color = {uiColor.x,uiColor.y,uiColor.z,uiColor.w};
        stride = primitive->stride;
        visable = true;
    }
    bool getVisable() const {return visable;}
    TopoType getType() const {return type;}
    const primitiveIdentifier* getIdentifier() const{return identifier;}
    GLsizei getStride() const{return stride;}
    void setColor(const glm::vec4 setColor){style.color = {setColor.x,setColor.y,setColor.z,setColor.w};}
    GLenum getShape() const {return shape;}
    const pVertexArray& getVertexArray() const {return refVertex;}
    const indexArray& getVertexIndex() const{return vertexIndex;}
    pVertexArray refVertex;
    virtual bool cursorSelectDetect(GLdouble xpos,GLdouble ypos) = 0;
    glm::vec2 getGeoCenter() const{return geoCenter;}
    glm::vec2 getRotateCenter() const{return rotateCenter;};
protected:
    void bindEBObuffer(){
        glGenBuffers(1, &EBO);
        glBindVertexArray(identifier->VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,  static_cast<GLsizei>(vertexIndex.size() * sizeof(GLuint)), static_cast<const void*>(vertexIndex.data()), GL_STATIC_DRAW);
        glBindVertexArray(0);
    }
    virtual void calcGeoCenter()=0;
    indexArray vertexIndex;
    glm::vec2 geoCenter,rotateCenter;
    bool visable;
    GLuint EBO;
    const primitiveIdentifier* identifier;
    Shader* shader;
    GLenum shape;
    GLsizei stride;
    struct Style{
        glm::vec4 color;
    } style;
    TopoType type;
    void setColor(bool highlighted);
    //ShaderPara style;
};

class Point: public Element{
public:
    Point(Primitive* primitive,GLuint startIndex):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        this->pointSize = primitive->thickness;
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = GL_POINTS;
        vertexIndex = {startIndex};
        calcGeoCenter();
        type = TopoType::point;
        bindEBObuffer();
    }
    friend class Line;
    friend class Face;
    glm::vec2 getCenterLocation() const{return geoCenter;}
    bool cursorSelectDetect(GLdouble xpos,GLdouble ypos);
    void draw(bool highlighted);
protected:
    void calcGeoCenter(){
        geoCenter.x = (*refVertex)[0];
        geoCenter.y = (*refVertex)[1];
        rotateCenter = geoCenter;
    }
private:
    GLfloat pointSize;
};
class Line: public Element{
public:
    Line(Primitive* primitive,GLuint startIndex,GLuint endIndex):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        this->lineWidth = primitive->thickness;
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = GL_LINES;
        vertexIndex = {startIndex,endIndex};
        point[0] = std::make_shared<Point>(primitive,vertexIndex[0]);
        primitive->elementList.push_back(point[0]);
        point[1] = std::make_shared<Point>(primitive,vertexIndex[1]);
        primitive->elementList.push_back(point[1]);
        calcGeoCenter();
        type = TopoType::line;
        bindEBObuffer();
    }
    friend class Face;
    glm::vec2 getCenterLocation() const{return geoCenter;}
    bool cursorSelectDetect(GLdouble xpos,GLdouble ypos);
    void draw(bool highlighted);
protected:
    void calcGeoCenter(){
        geoCenter.x = 0;
        geoCenter.y = 0;
        glm::vec2 centerLoc1 = (*point[0]).getCenterLocation(),centerLoc2 = (*point[1]).getCenterLocation();
        geoCenter.x = (centerLoc1.x + centerLoc2.x)/2;
        geoCenter.y = (centerLoc1.y + centerLoc2.y)/2;
        rotateCenter = geoCenter;
    }
private:
    GLfloat lineWidth;
    std::shared_ptr<Point> point[2];
};
typedef std::shared_ptr<Line> pLine;
class Face: public Element{
public:
    Face(Primitive* primitive):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = primitive->shape;
        const int n =  static_cast<int>((*refVertex).size()/stride);
        for (int i = 0; i<n-1; i++){
            vertexIndex.push_back(i);
            line.push_back(std::make_shared<Line>(primitive,i,i+1));
            primitive->elementList.push_back(line.back());
        }
        vertexIndex.push_back(n-1);
        line.push_back(std::make_shared<Line>(primitive,n-1,0));
        primitive->elementList.push_back(line.back());
        calcGeoCenter();
        type = TopoType::face;
        bindEBObuffer();
    }
    bool cursorSelectDetect(GLdouble xpos,GLdouble ypos);
    void draw(bool highlighted);
protected:
    void calcGeoCenter(){
        geoCenter.x = 0;
        geoCenter.y = 0;
        const int vertexNum = static_cast<int>(vertexIndex.size());
        for (auto it = line.begin(); it!=line.end(); it++){
            const glm::vec2 location = (*(*it)).getCenterLocation();
            geoCenter.x += location.x;
            geoCenter.y += location.y;
        }
        geoCenter.x /= vertexNum;
        geoCenter.y /= vertexNum;
        rotateCenter = geoCenter;
    }
private:
    std::vector<pLine> line;
};
int outboundDetect(pElement outbound);
}
#endif /* shape_hpp */

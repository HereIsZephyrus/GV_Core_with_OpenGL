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
#include "commander.hpp"
#include "factor.hpp"
typedef std::shared_ptr<vertexArray> pVertexArray;
void createTopoElements(Primitive* lastpPrimitive);
void updateTopoElements(Primitive* lastpPrimitive);
void changePrimitiveAttribute(Primitive* rawPrimitive);
GLfloat distancePointToLineSQ(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2, GLfloat x0, GLfloat y0);
namespace pr {
void updateIndex(Primitive*);
enum class TopoType{
    point,
    line,
    face,
    curve,
    diagnoal,
    outBound,
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
    virtual bool cursorSelectDetect(GLdouble xpos,GLdouble ypos) = 0;
    glm::vec2 getGeoCenter() const{return geoCenter;}
protected:
    void bindEBObuffer(){
        glGenBuffers(1, &EBO);
        glBindVertexArray(identifier->VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,  static_cast<GLsizei>(vertexIndex.size() * sizeof(GLuint)), static_cast<const void*>(vertexIndex.data()), GL_STATIC_DRAW);
        glBindVertexArray(0);
    }
    virtual void calcGeoCenter()=0;
    pVertexArray refVertex;
    indexArray vertexIndex;
    glm::vec2 geoCenter;
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
    Point(Primitive* primitive,GLuint startIndex,bool notShowLineStyle = false,bool visable = true):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        if (notShowLineStyle)
            this->pointSize = 2.0f;
        else
            this->pointSize = primitive->pointsize;
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = GL_POINTS;
        vertexIndex = {startIndex};
        calcGeoCenter();
        type = TopoType::point;
        this->visable = visable;
        bindEBObuffer();
    }
    friend class Line;
    friend class Face;
    bool cursorSelectDetect(GLdouble xpos,GLdouble ypos);
    void draw(bool highlighted);
    void setPointSize(GLfloat newSize){pointSize = newSize;}
protected:
    void calcGeoCenter(){
        geoCenter.x = (*refVertex)[vertexIndex[0]* stride];
        geoCenter.y = (*refVertex)[vertexIndex[0]* stride + 1];
    }
private:
    GLfloat pointSize;
};
typedef std::shared_ptr<Point> pPoint;
class Line: public Element{
public:
    Line(Primitive* primitive,GLuint startIndex,GLuint endIndex,bool notShowLineStyle = false,bool visable = true):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        if (notShowLineStyle)
            this->lineWidth = 2.0f;
        else
            this->lineWidth = primitive->thickness;
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = GL_LINES;
        vertexIndex = {startIndex,endIndex};
        point[0] = std::make_shared<Point>(primitive,vertexIndex[0],notShowLineStyle);
        primitive->elementList.push_back(point[0]);
        point[1] = std::make_shared<Point>(primitive,vertexIndex[1],notShowLineStyle);
        primitive->elementList.push_back(point[1]);
        calcGeoCenter();
        type = TopoType::line;
        this->visable = visable;
        bindEBObuffer();
    }
    friend class Face;
    glm::vec2 getCenterLocation() const{return geoCenter;}
    bool cursorSelectDetect(GLdouble xpos,GLdouble ypos);
    void draw(bool highlighted);
    void setLineWidth(GLfloat newWidth){lineWidth = newWidth;}
protected:
    void calcGeoCenter(){
        geoCenter.x = 0;
        geoCenter.y = 0;
        glm::vec2 centerLoc1 = point[0]->getGeoCenter(),centerLoc2 = point[1]->getGeoCenter();
        geoCenter.x = (centerLoc1.x + centerLoc2.x)/2;
        geoCenter.y = (centerLoc1.y + centerLoc2.y)/2;
    }
private:
    GLfloat lineWidth;
    pPoint point[2];
};
typedef std::shared_ptr<Line> pLine;
class Face: public Element{
public:
    Face(Primitive* primitive,bool visable = true):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = primitive->shape;
        const int n =  static_cast<int>((*refVertex).size()/stride);
        bool notShowLineStyle = !(shape == GL_LINE_STRIP || shape == GL_LINE_LOOP);
        for (int i = 0; i<n-1; i++){
            vertexIndex.push_back(i);
            line.push_back(std::make_shared<Line>(primitive,i,i+1,notShowLineStyle));
            primitive->elementList.push_back(line.back());
        }
        vertexIndex.push_back(n-1);
        line.push_back(std::make_shared<Line>(primitive,n-1,0,notShowLineStyle));
        primitive->elementList.push_back(line.back());
        calcGeoCenter();
        type = TopoType::face;
        this->visable = visable;
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
    }
private:
    std::vector<pLine> line;
};
class Curve: public Element{
public:
    Curve(Primitive* primitive,bool notShowLineStyle = false,bool visable = true):
    Element(primitive){
        ShaderStyle& style = ShaderStyle::getStyle();
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        if (notShowLineStyle)
            this->lineWidth = 5.0f;
        else
            this->lineWidth = primitive->thickness;
        shape = primitive->shape;
        const int n =  static_cast<int>((*refVertex).size()/stride);
        for (int i = 0; i< n; i++){
            vertexIndex.push_back(i);
            controlPoints.push_back(std::make_shared<Point>(primitive,vertexIndex.back()));
            primitive->elementList.push_back(controlPoints.back());
        }
        calcGeoCenter();
        type = TopoType::curve;
        this->visable = visable;
        bindEBObuffer();
        //std::cout<<controlPoints.size()<<std::endl;
    }
    bool cursorSelectDetect(GLdouble xpos,GLdouble ypos);
    void draw(bool highlighted);
protected:
    void calcGeoCenter(){
        geoCenter.x = 0;
        geoCenter.y = 0;
        const int vertexNum = static_cast<int>(vertexIndex.size());
        for (auto it = (*refVertex).begin(); it != (*refVertex).end(); it +=stride){
            geoCenter.x += *(it);
            geoCenter.y += *(it+1);
        }
        geoCenter.x /= vertexNum;
        geoCenter.y /= vertexNum;
    }
private:
    GLfloat lineWidth;
    std::vector<pPoint> controlPoints;
};
class Diagnoal: public Element{
public:
    Diagnoal(Primitive* primitive ,bool isCircle,GLuint startIndex = 0,GLuint endIndex = 1,bool notShowLineStyle = false,bool visable = true):
    Element(primitive),isCircle(isCircle){
        ShaderStyle& style = ShaderStyle::getStyle();
        if (notShowLineStyle)
            this->lineWidth = 5.0f;
        else
            this->lineWidth = primitive->thickness;
        this->isFill = style.toFill;
        this->style.color = {style.drawColor.x,style.drawColor.y,style.drawColor.z,style.drawColor.w};
        shape = GL_LINES;
        vertexIndex = {startIndex,endIndex};
        point[0] = std::make_shared<Point>(primitive,vertexIndex[startIndex],true,false);
        point[1] = std::make_shared<Point>(primitive,vertexIndex[endIndex],true,false);
        calcGeoCenter();
        type = TopoType::diagnoal;
        this->visable = visable;
        bindEBObuffer();
    }
    glm::vec2 getCenterLocation() const{return geoCenter;}
    bool cursorSelectDetect(GLdouble xpos,GLdouble ypos);
    void draw(bool highlighted);
    void setLineWidth(GLfloat newWidth){lineWidth = newWidth;}
protected:
    void calcGeoCenter(){
        geoCenter.x = 0;
        geoCenter.y = 0;
        glm::vec2 centerLoc1 = point[0]->getGeoCenter(),centerLoc2 = point[1]->getGeoCenter();
        geoCenter.x = (centerLoc1.x + centerLoc2.x)/2;
        geoCenter.y = (centerLoc1.y + centerLoc2.y)/2;
    }
private:
    GLfloat lineWidth;
    pPoint point[2];
    bool isCircle,isFill;
};
class OutBound{
public:
    OutBound(GLfloat const minX,GLfloat const minY,GLfloat const maxX,GLfloat const maxY,glm::mat3* transMat){
        vertices = {minX, minY, 0.0,minX, maxY, 0.0,maxX, maxY, 0.0,maxX, minY, 0.0,};
        geoCenter = {(minX + maxX)/2,(minY + maxY)/2};
        rotateCenter = geoCenter;
        refTransMat = transMat;
        size = {maxX - minX, maxY - minY, 0.0f};
    }
    glm::vec2 getGeocenter() const{return geoCenter;}
    glm::vec2 getRotateCenter() const{return rotateCenter;}
    bool cursorSelectDetect(GLdouble xpos,GLdouble ypos);
    void draw(bool highlighted);
    glm::mat3* getTransmat(){return refTransMat;}
    int cursorDetect(GLdouble xpos,GLdouble ypos);
    const glm::vec3 getSize(){return size;}
    const GLfloat getMinX(){return vertices[0];}
    const GLfloat getMinY(){return vertices[1];}
    const GLfloat getMaxX(){return vertices[6];}
    const GLfloat getMaxY(){return vertices[7];}
private:
    glm::mat3* refTransMat;
    glm::vec2 geoCenter,rotateCenter;
    vertexArray vertices;
    glm::vec3 size;
};
//typedef std::shared_ptr<Diagnoal> pDiagnoal;
int outboundDetect(pElement outbound);
}//namespace pr
#endif /* shape_hpp */

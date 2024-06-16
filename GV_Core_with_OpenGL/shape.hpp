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
namespace pr {
//don't rearrange point/line/face num -- don't need to tackle so much elements for now.
//extern GLuint pointNum;
//extern GLuint lineNum;
//extern GLuint faceNum;
extern GLuint elementNum;
class Element{
public:
protected:
    virtual void calcGeoCenter()=0;
    std::vector<int> vertexIndex;
    pVertexArray refVertex;
    glm::vec2 centerLocation;
};
class Point:  Element{
public:
    Point(const Primitive* primitive,int startIndex):
    pointSize(primitive->shader->thickness),
    color(primitive->shader->color){
        refVertex = std::make_shared<vertexArray>(primitive->vertices);
        vertexIndex = {startIndex};
        calcGeoCenter();
        id  = ++elementNum;
    }
    glm::vec2 getCenterLocation() const{return centerLocation;}
protected:
    void calcGeoCenter(){
        centerLocation.x = (*refVertex)[0];
        centerLocation.y = (*refVertex)[1];
    }
    GLuint id;
private:
    GLfloat& pointSize;
    glm::vec4& color;
};
class Line: Element{
public:
    Line(const Primitive* primitive,int startIndex):
    lineWidth(primitive->shader->thickness),
    color(primitive->shader->color){
        refVertex = std::make_shared<vertexArray>(primitive->vertices);
        vertexIndex = {startIndex,startIndex+1};
        point[0] = std::make_shared<Point>(primitive,vertexIndex[0]);
        point[1] = std::make_shared<Point>(primitive,vertexIndex[1]);
        calcGeoCenter();
        id = ++ elementNum;
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
    GLfloat& lineWidth;
    glm::vec4& color;
    std::shared_ptr<Point> point[2];
};
typedef std::shared_ptr<Line> pLine;
class Face: Element{
public:
    Face(const Primitive* primitive):
    color(primitive->shader->color){
        refVertex = std::make_shared<vertexArray>(primitive->vertices);
        const GLsizei stride = primitive->stride;
        for (int i = 0; i<(*refVertex).size()/stride; i++){
            vertexIndex.push_back(i);
            line.push_back(std::make_shared<Line>(primitive,i));
        }
        calcGeoCenter();
        id = ++ elementNum;
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
    glm::vec4& color;
    std::vector<pLine> line;
};
}
#endif /* shape_hpp */

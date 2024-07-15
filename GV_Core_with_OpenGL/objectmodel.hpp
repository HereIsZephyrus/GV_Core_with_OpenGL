//
//  objectmodel.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 7/8/24.
//

#ifndef objectmodel_hpp
#define objectmodel_hpp

#include <algorithm>
#include <memory>
#include <vector>
#include <map>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "primitive.hpp"
#include "rendering.hpp"
#include "camera.hpp"

struct Partition{
    primitiveIdentifier identifier;
    unsigned int layer,vertexNum;
    glm::vec3 color,centerPos;
    bool operator<(const Partition& other) const {
        return layer < other.layer;
    }
};
struct Object{
    vertexArray vertices;
    glm::vec3 color,centerPos;
};
typedef std::vector<Object> ObjectArray;
class ObjectModel{
public:
    ObjectModel(){
        this->shader = nullptr;
        this->flipShader = nullptr;
        this->scale = 0.0f;
        this->objectPosition = glm::vec3(0.0f,0.0f,0.0f);
    }
    ObjectModel(Shader* shader,Shader* filpShader,GLfloat scale = 5.0,glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f)){
        this->shader = shader;
        this->flipShader = filpShader;
        this->scale = scale;
        this->objectPosition = position;
    }
    ~ObjectModel(){partitions.clear();}
    void draw();
    void addPrimitive(const Object& object,unsigned int layer);
    void addPrimitive(const ObjectArray& objects,unsigned int layer);
    void setPosition(const glm::vec3 position){objectPosition = position;}
    void setScale(const int markerSize){scale = markerSize;}
private:
    std::vector<Partition> partitions;
    void useShader(Shader* activeShader);
    Shader* shader;
    Shader* flipShader;
    glm::vec3 objectPosition;
    GLfloat scale;
};
//typedef std::unique_ptr<ObjectModel> pObjectModel;

void initObject();
namespace obj {
extern std::map<std::string,ObjectModel> markers;
extern std::vector<ObjectModel> objectList;
static glm::vec3 colorInt2Float(glm::vec3 color){
    return glm::vec3(color.x/255.0f,color.y/255.0f,color.z/255.0f);
}
const Object frontBase = {
    {-8.0,0.0,0.0,0.0,0.0,0.0,-8.0,0.4,0.0,0.0,0.4,0.0},
    colorInt2Float(glm::vec3(243,205,84)),
    glm::vec3(0.0,-5.0,0.0),
};
const std::vector<vertexArray > groundVertices = {
{-5.0,0.0,0.0,0.0,0.0,0.0,-8.0,-1.6,0.0,0.0,-1.6,0.0},
{-2.0,0,0.0,0.0,0,0.0,-2.0,-0.3,0.0,0.0,-0.3,0.0},
{-1.7,0.0,0.0,0.0,0.0,0.0,-1.7,-0.3,0.0,0.0,-0.3,0.0},
{-0.2,0.0,0.0,0.2,0.0,0.0,0.0,0.7,0.0,0.4,0.7,0.0},
};
const std::vector<glm::vec3> groundColors = {
colorInt2Float(glm::vec3(160,72,72)),
colorInt2Float(glm::vec3(243,205,84)),
colorInt2Float(glm::vec3(160,72,72)),
colorInt2Float(glm::vec3(243,205,84)),
};
const std::vector<glm::vec3> groundCenters = {
glm::vec3(0.0,-3.0,0.0),
glm::vec3(0.0,-5.3,0.0),
glm::vec3(0.0,-5.0,0.0),
glm::vec3(-1.8,-5.3,0.0),
};
const std::vector<vertexArray > wallVertices = {
{0.0,0.0,0.0,0.0,-3.4,0.0,-3.0,0.0,0.0,-3.0,-5.4,0.0},
{0.0,0.0,0.0,0.0,-1.6,0.0,-0.8,-0.4,0.0,-0.8,-2.0,0.0},
{0.0,0.0,0.0,0.0,2.0,0.0,-0.2,0.0,0.0,-0.2,2.0,0.0},
{0.0,0.0,0.0,0.0,1.6,0.0,-0.2,0.0,0.0,-0.2,1.6,0.0},
{0.0,0.0,0.0,-0.2,0.0,0.0,-0.1,0.5,0.0,-0.3,0.5,0.0},
{0.0,0.0,0.0,-0.2,0.0,0.0,0.0,0.7,0.0,-0.2,0.7,0.0},
{0.0,0.0,0.0,-0.2,0.0,0.0,-0.1,0.4,0.0,-0.3,0.4,0.0},
{0.0,0.0,0.0,-0.2,0.0,0.0,0.0,0.1,0.0,-0.2,0.1,0.0},
};
const std::vector<glm::vec3> wallColors = {
colorInt2Float(glm::vec3(228,139,51)),
colorInt2Float(glm::vec3(51,171,255)),
colorInt2Float(glm::vec3(235,196,86)),
colorInt2Float(glm::vec3(71,243,255)),
colorInt2Float(glm::vec3(235,196,86)),
colorInt2Float(glm::vec3(235,196,86)),
colorInt2Float(glm::vec3(235,196,86)),
colorInt2Float(glm::vec3(235,196,86)),
};
const std::vector<glm::vec3> wallCenters = {
glm::vec3(-5.0,0.4,0.0),
glm::vec3(-7.0,-0.8,0.0),
glm::vec3(-7.8,-4.6,0.0),
glm::vec3(-7.8,-2.8,0.0),
glm::vec3(-7.8,-1.2,0.0),
glm::vec3(-7.9,-0.7,0.0),
glm::vec3(-7.9,0.0,0.0),
glm::vec3(-8.0,0.4,0.0),
};
const std::vector<vertexArray > doorVertices = {
{-2.0,0.0,0.0,1.0,0.0,0.0,-2.0,-3.0,0.0,1.0,-3.0,0.0},
{-1.0,0.0,0.0,1.0,0.0,0.0,-1.0,-3.0,0.0,1.0,-3.0,0.0},
{-6.0,0.0,0.0,0.0,0.0,0.0,-6.0,-0.7,0.0,0.0,-0.7,0.0},
{-0.2,0.0,0.0,0.2,0.0,0.0,-0.2,-4.0,0.0,0.2,-4.0,0.0},
{-0.05,0.0,0.0,0.05,0.0,0.0,-0.05,-3.5,0.0,0.05,-3.5,0.0},
{-0.2,0.0,0.0,0.2,0.0,0.0,-0.5,0.6,0.0,0.5,0.6,0.0},
{-0.2,0.0,0.0,0.2,0.0,0.0,-0.5,0.6,0.0,0.5,0.6,0.0},
{0.0,0.0,0.0,0.0,0.4,0.0},
{0.0,0.0,0.0,0.0,-0.4,0.0},
};
const std::vector<glm::vec3> doorColors = {
colorInt2Float(glm::vec3(223,223,148)),
colorInt2Float(glm::vec3(220,75,44)),
colorInt2Float(glm::vec3(255,195,51)),
colorInt2Float(glm::vec3(255,93,51)),
colorInt2Float(glm::vec3(255,93,51)),
colorInt2Float(glm::vec3(255,237,51)),
colorInt2Float(glm::vec3(255,237,51)),
colorInt2Float(glm::vec3(0,0,0)),
colorInt2Float(glm::vec3(0,0,0)),
};
const std::vector<glm::vec3> doorCenters = {
glm::vec3(-3.0,0.0,0.0),
glm::vec3(-1.0,0.0,0.0),
glm::vec3(0.0,0.4,0.0),
glm::vec3(-1.6,0.0,0.0),
glm::vec3(-6.0,0.4,0.0),
glm::vec3(-1.6,-4.6,0.0),
glm::vec3(-6.0,-3.7,0.0),
glm::vec3(-1.6,0.0,0.0),
glm::vec3(-1.6,-4.0,0.0),
};
const std::vector<vertexArray > roofVertices = {
{-4.0,0.0,0.0,0.0,0.0,0.0,-4.0,-0.7,0.0,0.0,-0.7,0.0},
{0.0,0.0,0.0,0.0,0.7,0.0,1,-1,0.0,1.2,-0.5,0.0,},
{0.0,0.0,0.0,0.0,0.4,0.0,0.6,-1.5,0.0,3.0,-1.5,0.0,},
{0.0,0.0,0.0,0.0,0.2,0.0,0.2,-2.2,0.0,1.6,-2.0,0.0,},
{0.0,0.0,0.0,-1.0,1.0,0.0,-1.0,-1.0,0.0,0.0,-0.2,0.0,-3.5,0.0,0.0,-3.5,-2.0,0.0,-4.6,-2.5,0.0,-3.5,-0.5,0.0,-4.6,-0.5,0.0,},
{0.0,0.0,0.0,-8.0,0.0,0.0,0.0,2.5,0.0,-3.5,2.5,0.0},
{0.0,0.0,0.0,-8.0,0.0,0.0,0.0,0.1,0.0,-8.0,0.1,0.0},
{-0.1,0.0,0.0,-0.8,1.5,0.0,0.1,0.0,0.0,0.4,1.5,0.0,},
{-0.6,0.0,0.0,0.6,0.0,0.0,-0.6,0.5,0.0,0.6,0.5,0.0,},
{-0.6,0.0,0.0,0.6,0.0,0.0,0.0,0.6,0.0,},
{-0.4,0.0,0.0,0.4,0.0,0.0,0.0,0.4,0.0,},
{-0.6,0.0,0.0,0.6,0.0,0.0,0.0,0.6,0.0,},
{-0.2,0.0,0.0,-0.6,0.7,0.0,0.2,0.0,0.0,0.2,0.7,0.0,},
{-0.1,0.0,0.0,-0.8,0.7,0.0,0.1,0.0,0.0,0.4,0.7,0.0,}
};
const std::vector<glm::vec3> roofColors = {
colorInt2Float(glm::vec3(223,223,148)),
colorInt2Float(glm::vec3(218,115,0)),
colorInt2Float(glm::vec3(218,115,0)),
colorInt2Float(glm::vec3(218,115,0)),
colorInt2Float(glm::vec3(244,234,139)),
colorInt2Float(glm::vec3(224,115,92)),
colorInt2Float(glm::vec3(223,223,148)),
colorInt2Float(glm::vec3(201,68,27)),
colorInt2Float(glm::vec3(240,68,62)),
colorInt2Float(glm::vec3(254,137,88)),
colorInt2Float(glm::vec3(254,137,88)),
colorInt2Float(glm::vec3(254,137,88)),
colorInt2Float(glm::vec3(201,68,27)),
colorInt2Float(glm::vec3(201,68,27)),
};
const std::vector<glm::vec3> roofCenters = {
glm::vec3(0.0,3.7,0.0),
glm::vec3(-8.3,3.1,0.0),
glm::vec3(-7.0,3.6,0.0),
glm::vec3(-5.2,4.1,0.0),
glm::vec3(-3.5,3.0,0.0),
glm::vec3(0.0,0.5,0.0),
glm::vec3(0.0,0.4,0.0),
glm::vec3(-8.1,0.5,0.0),
glm::vec3(-8.3,2.0,0.0),
glm::vec3(-8.3,2.5,0.0),
glm::vec3(-7.0,3.2,0.0),
glm::vec3(-5.2,3.5,0.0),
glm::vec3(-6.8,2.5,0.0),
glm::vec3(-5.0,2.8,0.0),
};
void initLogo();
};
#endif /* objectmodel_hpp */
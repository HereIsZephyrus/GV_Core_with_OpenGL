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
class ObjectModel{
public:
    ObjectModel(Shader* shader,glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f)){
        this->shader = shader;
        this->scale = 10;
        this->objectPosition = position;
    }
    ~ObjectModel(){partitions.clear();}
    void draw();
    void addPrimitive(const vertexArray& vertices,const glm::vec3& color,const glm::vec3& centerPos,unsigned int layer);
    void addPrimitive(const Object& object,unsigned int layer);
    void setPosition(const glm::vec3 position){objectPosition = position;}
private:
    std::vector<Partition> partitions;
    void useShader();
    Shader* shader;
    glm::vec3 objectPosition;
    int scale;
};
//typedef std::unique_ptr<ObjectModel> pObjectModel;

void initObject();
namespace obj {
extern std::map<std::string,ObjectModel> markers;
extern std::vector<ObjectModel> objectList;
static glm::vec3 colorInt2Float(glm::vec3 color){
    return glm::vec3(color.x/255.0f,color.y/255.0f,color.z/255.0f);
}
const vertexArray objVertices[] = {
    {0.0,0.0,0.0,-8.0,0.0,0.0,0.0,2.5,0.0,-3.5,2.5,0.0},
    {-8.0,0.0,0.0,0.0,0.0,0.0,0.0,0.1,0.0,-8.0,0.1,0.0},
    {0.0,0.0,0.0,0.0,-3.4,0.0,-3.0,-5.4,0.0,-3.0,0.0,0.0},
    {-5.0,0.0,0.0,0.0,0.0,0.0,-8.0,-1.6,0.0,0.0,-1.6,0.0},
    {-2.0,0.0,0.0,1.0,0.0,0.0,-2.0,-3.0,0.0,1.0,-3.0,0.0},
    {-1.0,0.0,0.0,0.0,0.0,0.0,-1.0,-3.0,0.0,0.0,-3.0,0.0},
    {-6.0,0.0,0.0,0.0,0.0,0.0,-6.0,-0.7,0.0,0.0,-0.7,0.0},
    {-0.2,0.0,0.0,0.2,0.0,0.0,-0.2,-4.0,0.0,-0.2,-4.0,0.0},
    {-0.05,0.0,0.0,0.05,0.0,0.0,-0.05,-5.0,0.0,0.05,-5.0,0.0},
    {-0.2,0.0,0.0,0.2,0.0,0.0,-0.5,0.6,0.0,0.5,0.6,0.0},
    {-0.2,0.0,0.0,0.2,0.0,0.0,-0.5,0.6,0.0,0.5,0.6,0.0},
    {0.0,0.0,0.0,0.0,0.4,0.0},
    {0.0,0.0,0.0,0.0,-0.4,0.0},
    {-2.0,0,0.0,0.0,0,0.0,-2.0,-0.3,0.0,0.0,-0.3,0.0},
    {-8.0,0.0,0.0,0.0,0.0,0.0,-8.0,0.4,0.0,0.0,0.4,0.0},
    {-1.7,0.0,0.0,0.0,0.0,0.0,-1.7,-0.3,0.0,0.0,-0.3,0.0},
    {-0.2,0.0,0.0,0.2,0.0,0.0,0.0,0.7,0.0,0.4,0.7,0.0},
};
const glm::vec3 colorVertices[] = {
    colorInt2Float(glm::vec3(224,115,92)),
    colorInt2Float(glm::vec3(223,223,148)),
    colorInt2Float(glm::vec3(215,196,86)),
    colorInt2Float(glm::vec3(160,72,72)),
    colorInt2Float(glm::vec3(223,223,148)),
    colorInt2Float(glm::vec3(220,75,44)),
    colorInt2Float(glm::vec3(166,166,96)),
    colorInt2Float(glm::vec3(255,93,51)),
    colorInt2Float(glm::vec3(255,93,51)),
    colorInt2Float(glm::vec3(255,237,51)),
    colorInt2Float(glm::vec3(255,237,51)),
    colorInt2Float(glm::vec3(0,0,0)),
    colorInt2Float(glm::vec3(0,0,0)),
    colorInt2Float(glm::vec3(243,205,84)),
    colorInt2Float(glm::vec3(243,205,84)),
    colorInt2Float(glm::vec3(160,72,72)),
    colorInt2Float(glm::vec3(243,205,84)),
};
const glm::vec3 centerPos[] = {
    glm::vec3(0.0,0.5,0.0),
    glm::vec3(0.0,0.4,0.0),
    glm::vec3(-5.0,0.4,0.0),
    glm::vec3(0.0,-3.0,0.0),
    glm::vec3(-3.0,0.0,0.0),
    glm::vec3(-1.0,0.0,0.0),
    glm::vec3(0.0,0.4,0.0),
    glm::vec3(-1.6,0.0,0.0),
    glm::vec3(-6.0,0.4,0.0),
    glm::vec3(-1.6,-5.0,0.0),
    glm::vec3(-6.0,-3.5,0.0),
    glm::vec3(-1.6,0.0,0.0),
    glm::vec3(-1.6,-4.0,0.0),
    glm::vec3(0.0,-5.3,0.0),
    glm::vec3(0.0,-5.0,0.0),
    glm::vec3(0.0,-5.0,0.0),
    glm::vec3(-1.8,-5.3,0.0),
};
};
#endif /* objectmodel_hpp */

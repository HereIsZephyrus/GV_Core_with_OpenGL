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
class object{
public:
    object(Shader* shader,glm::vec3 position){
        this->shader = shader;
        this->objectPosition = position;
    }
    ~object(){partitions.clear();}
    void draw();
    void addPrimitive(const vertexArray& vertices,const glm::vec3& color,const glm::vec3& centerPos,unsigned int layer);
private:
    std::vector<Partition> partitions;
    void useShader();
    Shader* shader;
    glm::vec3 objectPosition;
};
#endif /* objectmodel_hpp */

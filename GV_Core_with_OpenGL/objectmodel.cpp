//
//  objectmodel.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 7/8/24.
//

#include "objectmodel.hpp"
void object::addPrimitive(const vertexArray& vertices,const glm::vec3& color,const glm::vec3& centerPos,unsigned int layer){
    primitiveIdentifier identifier = {0,0};
    glGenVertexArrays(1,&identifier.VAO);
    glGenBuffers(1,&identifier.VBO);
    glBindVertexArray(identifier.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, identifier.VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(GLfloat)) ,static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    Partition newPartition{identifier,layer,static_cast<unsigned int>(vertices.size()/3),color,centerPos};
    auto it = std::upper_bound(partitions.begin(), partitions.end(), newPartition);
    partitions.insert(it, newPartition);
}
void object::useShader(){
    if (shader == nullptr){
        std::cerr<<"havn't bind shader";
        return;
    }
    else
        shader ->use();
    //camera
    GLuint projectionLoc = glGetUniformLocation(shader->program, "projection");
    GLuint viewLoc = glGetUniformLocation(shader->program, "view");
    
    Camera2D& camera = Camera2D::getView();
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
}
void object::draw(){
    GLuint modelLoc = glGetUniformLocation(shader->program,"model");
    GLuint colorLoc = glGetUniformLocation(shader->program,"setColor");
    for (auto it = partitions.begin(); it != partitions.end(); it++){
        glm::mat4 model = glm::mat4(1.0f);
        model[3][0] = (*it).centerPos.x + objectPosition.x;
        model[3][1] = (*it).centerPos.y + objectPosition.y;
        model[3][2] = (*it).centerPos.z + objectPosition.z;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        const glm::vec3 color = (*it).color;
        glUniform4f(colorLoc,color.x,color.y,color.z,1.0f);
        glBindVertexArray((*it).identifier.VAO);
        if ((*it).vertexNum == 2)
            glDrawArrays(GL_LINE, 0, (*it).vertexNum);
        else
            glDrawArrays(GL_TRIANGLE_STRIP, 0, (*it).vertexNum);
        glBindVertexArray(0);
    }
}

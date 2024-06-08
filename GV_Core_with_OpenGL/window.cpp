//
//  window.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/8/24.
//
#include <iostream>
#include "window.hpp"

namespace Renders {
const GLchar *vertexShaderSource = "#version 410 core\n"
"layout (location = 0) in vec3 position;\n"
"void main(){gl_Position = vec4( position.x,position.y,position.z, 1.0);}\n";

const GLchar *fragmentShaderSource = "#version 410 core\n"
"out vec4 color;\n"
"void main( ){color = vec4( 1.0f, 0.5f, 0.2f, 1.0f );}";

int buildVertexRender(GLFWwindow* &window,GLuint& vertexShader){
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&Renders::vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    
    GLint success;
    GLchar infolog[512];
    
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if (!success){
        glGetShaderInfoLog(vertexShader,512,NULL,infolog);
        std::cerr<< "shader::vertex::compilication_failed\n"<<infolog<<std::endl;
        return -1;
    }
    return 0;
}
int buildFragementRender(GLFWwindow* &window,GLuint& fragmentShader){
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&Renders::fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    
    GLint success;
    GLchar infolog[512];
    
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if (!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,infolog);
        std::cerr<< "shader::fragment::conpilication_failed\n"<<infolog<<std::endl;
        return -1;
    }
    return 0;
}
int buildShaderProgram(GLFWwindow* &window,GLuint& shaderProgram){
    
    GLuint vertexShader;
    if (Renders::buildVertexRender(window,vertexShader)!=0)
        return -1;
    
    GLuint fragmentShader;
    if (Renders::buildFragementRender(window,fragmentShader)!=0)
        return -1;
    
    shaderProgram  = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    GLchar infolog[512];
        
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if (!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,infolog);
        std::cerr<< "shader::program::linking_failed\n"<<infolog<<std::endl;
        return -2;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
}

}

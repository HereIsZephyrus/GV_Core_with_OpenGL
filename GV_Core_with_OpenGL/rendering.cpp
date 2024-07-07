//
//  rendering.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/8/24.
//
#include <iomanip>
#include "rendering.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "commander.hpp"

namespace rd{
std::map<std::string,pShader > namedShader;
std::vector<pShader> mainShaderList;
//Shader* defaultShader;

GLchar* filePath(const char* fileName){
    //checkSourceRelevantPath();
    const char * tcbsearchPath ="../../../../../../../../Program/GV_Core_with_OpenGL/resources/";
    GLchar* resource = new char[strlen(tcbsearchPath) + strlen(fileName) + 1];
    //strcpy_s(resource,sizeof(resource), tcbsearchPath);
    //strcat_s(resource, sizeof(resource), fileName);
    strcpy(resource, tcbsearchPath);
    strcat(resource, fileName);
    //std::cout<<resource<<std::endl;
    return resource;
}
};

void Shader::attchVertexShader(std::string vertexShader){
    const GLchar* vsShaderCode = vertexShader.c_str();
    GLint success;
    GLchar infoLog[512];
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glAttachShader(program, vertex);
    glDeleteShader(vertex);
}
void Shader::attchVertexShader(const GLchar* vertexPath){
    std::string vertexShader = readGLSLfile(vertexPath);
    attchVertexShader(vertexShader);
}
void Shader::attchFragmentShader(std::string fragmentShader){
    const GLchar* fragShaderCode = fragmentShader.c_str();
    GLint success;
    GLchar infoLog[512];
    GLuint fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glAttachShader(program, fragment);
    glDeleteShader(fragment);
}
void Shader::attchFragmentShader(const GLchar* fragmentPath){
    std::string fragmentShader = readGLSLfile(fragmentPath);
    attchFragmentShader(fragmentShader);
}
std::string Shader::readGLSLfile(const GLchar* filePath){
    std::string fileString;
    std::ifstream fileStream;
    fileStream.exceptions(std::ifstream::badbit);
    try {
        fileStream.open(filePath);
        if (!fileStream.is_open())
            std::cerr << "ERROR::SHADER::Failed_TO_READ_SHADERFILE." << std::endl;
        std::stringstream shaderStream;
        shaderStream << fileStream.rdbuf();
        fileStream.close();
        return shaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cerr<<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
}

void Shader::linkProgram(){
    GLint success;
    GLchar infoLog[512];
    glLinkProgram(program);
    // Print linking errors if any
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

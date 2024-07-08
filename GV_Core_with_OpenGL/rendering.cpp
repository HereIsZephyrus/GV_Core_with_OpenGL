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
void Shader::attchShader(std::string shader,GLuint type){
    const GLchar* shaderCode = shader.c_str();
    GLint success;
    GLchar infoLog[512];
    GLuint shaderProgram;
    shaderProgram = glCreateShader(type);
    glShaderSource(shaderProgram, 1, &shaderCode, NULL);
    glCompileShader(shaderProgram);
    // Print compile errors if any
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glAttachShader(program, shaderProgram);
    glDeleteShader(shaderProgram);
}
void Shader::attchShader(const GLchar* path,GLuint type){
    std::string shader = readGLSLfile(path);
    attchShader(shader,type);
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

void ShaderStyle::initStyle(){
    drawColor =  ImVec4(1.0f, 0.5f, 0.2f, 1.0f);
    thickness = 5.0f;
    pointsize = 5.0f;
    headType = LineHeadType::cube;
    lineType = LineType::fill;
}

//
//  rendering.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/8/24.
//
#include <iomanip>
#include "rendering.hpp"
#include "window.hpp"
namespace rd{
std::map<std::string,pShader > namedShader;
std::vector<pShader> mainShaderList;
Shader* defaultShader;

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
std::string geneateColorShader(const ImVec4& color){
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << color.x<<"f,"<<color.y<<"f,"<<color.z<<"f,"<<color.w;
    std::string  shaderCode = "#version 410 core\n"
    "out vec4 color;\n"
    "void main( ){\n"
    "color = vec4( " + ss.str() + ");\n}";
    //std::cout<<shaderCode<<std::endl;
    return shaderCode;
}

std::string singleVertices = "#version 410 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"uniform mat4 model;\n"
"void main() {\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\n";

std::string fillYellow = "#version 410 core\n"
"out vec4 color;\n"
"void main( ){color = vec4( 1.0f, 0.5f, 0.2f, 1.0f );}";

std::string fillWhite = "#version 410 core\n"
"out vec4 color;\n"
"void main( ){color = vec4( 1.0f, 1.0f, 1.0f, 1.0f );}";
};
Shader::Shader(std::string vertexShader,std::string fragmentShader){
    const GLchar* vShaderCode = vertexShader.c_str();
    const GLchar * fShaderCode = fragmentShader.c_str();
    this->Program = glCreateProgram();
    generateProgram(vShaderCode,fShaderCode,this->Program);
}
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath){
    std::string vertexShader = readGLSLfile(vertexPath);
    std::string fragmentShader = readGLSLfile(fragmentPath);
    const GLchar* vShaderCode = vertexShader.c_str();
    const GLchar * fShaderCode = fragmentShader.c_str();
    this->Program = glCreateProgram();
    generateProgram(vShaderCode,fShaderCode,this->Program);
}
Shader::Shader(const GLchar* vertexPath,std::string fragmentShader){
    std::string vertexShader = readGLSLfile(vertexPath);
    const GLchar* vShaderCode = vertexShader.c_str();
    const GLchar * fShaderCode = fragmentShader.c_str();
    this->Program = glCreateProgram();
    generateProgram(vShaderCode,fShaderCode,this->Program);
}
Shader::Shader(std::string vertexShader,const GLchar* fragmentPath){
    std::string fragmentShader = readGLSLfile(fragmentPath);
    const GLchar* vShaderCode = vertexShader.c_str();
    const GLchar * fShaderCode = fragmentShader.c_str();
    this->Program = glCreateProgram();
    generateProgram(vShaderCode,fShaderCode,this->Program);
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
    return "";
}

void Shader::generateProgram(const GLchar* vShaderCode,const GLchar * fShaderCode,GLuint& program){
    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Shader Program
    
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    // Print linking errors if any
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

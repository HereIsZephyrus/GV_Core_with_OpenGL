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

};

void  Shader::rend() {
    //glUseProgram(program);
    //color
    GLuint colorLoc = glGetUniformLocation(program,"setColor");
    glUniform4f(colorLoc,color.x,color.y,color.z,color.w);
    //thickness
    //GLuint  resLoc  = glGetUniformLocation(program, "resolution");
    //GLuint  thiLoc  = glGetUniformLocation(program, "thickness");
    //glUniform1f(thiLoc,thickness);
    //WindowParas& windowPara = WindowParas::getInstance();
    //glUniform2f(resLoc, windowPara.SCREEN_WIDTH,windowPara.SCREEN_HEIGHT);
    
    //camera
    GLuint projectionLoc = glGetUniformLocation(program, "projection");
    GLuint viewLoc = glGetUniformLocation(program, "view");
    GLuint modelLoc = glGetUniformLocation(program, "model");
    Camera2D& camera = Camera2D::getView();
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

Shader::Shader(ShaderStyle& style){
    thickness = style.thickness;
    color.x = style.drawColor.x;
    color.y = style.drawColor.y;
    color.z = style.drawColor.z;
    color.w = style.drawColor.w;
    this->program = glCreateProgram();
    
    //attchVertexShader(rd::filePath("singleVertices.vs"));
    //attchFragmentShader(rd::filePath("fillColor.frag"));
    //linkProgram();
}
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

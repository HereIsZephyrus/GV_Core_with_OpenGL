//
//  rendering.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/8/24.
//

#ifndef rendering_hpp
#define rendering_hpp
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ShaderStyle{
public:
    static ShaderStyle& getStyle(){
        static ShaderStyle instance;
        return instance;
    }
    ShaderStyle(const ShaderStyle&) = delete;
    void operator = (const ShaderStyle&) = delete;
    bool toFill;
    ImVec4 drawColor;
    float thickness,pointSize;
private:
    ShaderStyle(){}
};
class Shader{
public:
   Shader(const Shader&) = delete;
    void operator=(const Shader&) = delete;
    //Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    //Shader(std::string vertexShader,std::string fragmentShader);
    //Shader(const GLchar* vertexPath,std::string fragmentShader);
    //Shader(std::string vertexShader,const GLchar* fragmentPath);
    Shader(ShaderStyle& style);
    void  rend() ;
    void use(){
        glUseProgram(program);
    }
    void attchVertexShader(std::string vertexShader);
    void attchVertexShader(const GLchar* vertexPath);
    void attchFragmentShader(std::string fragmentShader);
    void attchFragmentShader(const GLchar* fragmentPath);
    void linkProgram();
private:
    GLfloat thickness;
    glm::vec4 color;
    GLuint program;
    std::string readGLSLfile(const GLchar* filePath);
};


typedef std::unique_ptr<Shader> pShader;
namespace rd{
extern std::string singleVertices;
extern std::string fillYellow;
extern std::string fillWhite;
extern std::map<std::string,pShader > namedShader;
extern std::vector<pShader> mainShaderList;
//extern Shader* defaultShader;
extern Shader* defaultShader;
GLchar* filePath(const char* fileName);
std::string geneateColorShader(const ImVec4& color);
};

#endif /* rendering_hpp */

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
    GLfloat thickness,pointsize;
private:
    ShaderStyle(){}
};
class Shader{
public:
   Shader(const Shader&) = delete;
    void operator=(const Shader&) = delete;
    Shader(){
        this->program = glCreateProgram();
    }
    void use(){
        glUseProgram(program);
    }
    void attchShader(std::string shader,GLuint type);
    void attchShader(const GLchar* path,GLuint type);
    void linkProgram();
    GLuint program;
private:
    std::string readGLSLfile(const GLchar* filePath);
};


typedef std::unique_ptr<Shader> pShader;
namespace previewStyle {
constexpr glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
}
namespace rd{
extern std::string singleVertices;
extern std::string fillYellow;
extern std::string fillWhite;
extern std::map<std::string,pShader > namedShader;
extern std::vector<pShader> mainShaderList;
//extern Shader* defaultShader;
//extern Shader* defaultShader;
GLchar* filePath(const char* fileName);
std::string geneateColorShader(const ImVec4& color);
};

#endif /* rendering_hpp */

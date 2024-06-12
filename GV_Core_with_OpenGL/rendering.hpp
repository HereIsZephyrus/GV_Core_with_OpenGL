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

#include <GL/glew.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Shader{
public:
   Shader(const Shader&) = delete;
    void operator=(const Shader&) = delete;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    Shader(std::string vertexShader,std::string fragmentShader);
    //Shader(){}
    void  rend() const{
        //std::cout<<"Render is running"<<std::endl;
        glUseProgram(this->Program);
    }
    GLuint getProgram(){
        return Program;
    }
private:
    GLuint Program;
};
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
    float thickness;
private:
    ShaderStyle(){}
};
typedef std::unique_ptr<Shader> pShader;
namespace rd{
extern std::string singleVertices;
extern std::string fillYellow;
extern std::string fillWhite;
extern std::map<std::string,pShader > shaders;
//extern Shader* defaultShader;
extern const Shader* defaultShader;
};

#endif /* rendering_hpp */

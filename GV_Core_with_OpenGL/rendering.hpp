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
    inline void rend(){
        //std::cout<<"Render is running"<<std::endl;
        glUseProgram(this->Program);
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

namespace rd{
extern std::string singleVertices;
extern std::string fillYellow;
};

#endif /* rendering_hpp */

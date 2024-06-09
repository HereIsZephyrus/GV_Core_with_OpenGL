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

class Shader{
public:
    Shader(const Shader&) = delete;
    void operator=(const Shader&) = delete;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    Shader(std::string vertexShader,std::string fragmentShader);
    
    void Rend(){glUseProgram(this->Program);}
private:
    GLuint Program;
};

namespace rd{

extern std::string singleVertices;
extern std::string fillYellow;
};

#endif /* rendering_hpp */

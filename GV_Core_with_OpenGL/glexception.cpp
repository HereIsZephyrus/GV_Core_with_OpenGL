//
//  exception.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/9/24.
//

#include "glexception.hpp"

void checkGLError(const char* stmt, const char* fname, int line) {
    GLenum err = glGetError();
    while (err != GL_NO_ERROR) {
        std::string error;
        switch (err) {
            case GL_INVALID_ENUM:
                error = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "GL_INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "GL_STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "GL_OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                error = "UNKNOWN_ERROR";
                break;
        }
        std::cerr << "GL_" << error.c_str() << " - " << stmt << " - " << fname << ":" << line << std::endl;
        err = glGetError();  // 再次调用以清除所有错误
    }
}

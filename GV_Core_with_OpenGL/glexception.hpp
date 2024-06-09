//
//  exception.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/9/24.
//

#ifndef glexception_hpp
#define glexception_hpp

#include <GL/glew.h>
#include <iostream>
#define CHECK_GL_ERROR(stmt) do { \
    stmt; \
    checkGLError(#stmt, __FILE__, __LINE__); \
} while (0)
void checkGLError(const char* stmt, const char* fname, int line) ;
#endif /* g;exception_hpp */

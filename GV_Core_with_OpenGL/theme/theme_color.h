//
//  theme_color.h
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//

#ifndef theme_color_h
#define theme_color_h

#include<GLFW/glfw3.h>

namespace tc{
static int primary = 1,secondary = 2, against = 3;
GLfloat background[] = {
    0.0f,0.0f,0.0f,1.0f, //primary
    0.1f,0.1f,0.1f,1.0f, //secondary
    1.0f,1.0f,1.0f,1.0f, //against
};
GLfloat text[] = {
    1.0f,1.0f,1.0f,1.0f, //primary
    0.9f,0.9f,0.9f,1.0f, //secondary
    0.0f,0.0f,0.0f,1.0f, //against
};
GLfloat sketch[] = {
    1.0f,1.0f,1.0f,1.0f, //primary
    0.9f,0.9f,0.9f,1.0f, //secondary
    0.0f,0.0f,0.0f,1.0f, //against
};
}

#endif /* theme_color_h */

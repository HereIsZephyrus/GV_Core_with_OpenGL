//
//  drawlines.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <cmath>
#include "drawlines.hpp"

void drawLine(const float &sX,const float &sY,const float &tX,const float &tY) {
    glClear(GL_COLOR_BUFFER_BIT);
    std::cout<<sX << ' '<<sY<<' '<<tX << ' '<<tY<<std::endl;
    float dx = tX - sX,dy = tY - sY,steps = std::max(std::abs(dx), std::abs(dy));
    float xinc = dx / steps;
    float yinc = dy / steps;
    float x = sX,y = sY;
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++,x += xinc,y += yinc) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(x, y);
    }
    glEnd();
    glfwSwapBuffers(glfwGetCurrentContext());
}

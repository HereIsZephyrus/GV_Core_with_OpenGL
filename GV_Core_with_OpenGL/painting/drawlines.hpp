//
//  drawlines.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//

#ifndef drawlines_hpp
#define drawlines_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <vector>
typedef std::pair<float, float>  fPoint;
extern std::vector<fPoint> lines;
void drawLine(const float &sX,const float &sY,const float &tX,const float &tY);
#endif /* drawlines_hpp */

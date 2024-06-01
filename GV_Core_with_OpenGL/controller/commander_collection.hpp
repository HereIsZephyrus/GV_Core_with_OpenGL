//
//  commander_collection.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//

#ifndef commander_collection_hpp
#define commander_collection_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "constants.h"
typedef std::pair<float, float>  fPoint;
extern std::vector<fPoint> lines;
void keyCommand(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) ;
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

#endif /* commander_collection_hpp */

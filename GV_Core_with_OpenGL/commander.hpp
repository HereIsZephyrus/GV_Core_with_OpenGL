//
//  commander_collection.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//

#ifndef commander_hpp
#define commander_hpp

#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

void keyBasicCommand(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseDrawLine(GLFWwindow* window, int button, int action, int mods);
//void mouseDrawLines(GLFWwindow* window, int button, int action, int mods);
//void mouseDrawCirles(GLFWwindow* window, int button, int action, int mods);

#endif /* commander_hpp */
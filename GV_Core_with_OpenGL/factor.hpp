//
//  factor.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/26/24.
//

#ifndef factor_hpp
#define factor_hpp
#include <memory>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "primitive.hpp"
#include "rendering.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void keyModsToggle(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseModsToggle(GLFWwindow* window, int button, int action, int mods);
void cursorDragingDetect(GLFWwindow* window, double xpos, double ypos);
void drawModsToggle(GLFWwindow* window, int button, int action, int mods);
void editPrimitive();
void viewScroll(GLFWwindow* window, double xoffset, double yoffset);
void processCursorTrace(GLFWwindow* window,double xpos, double ypos);
bool primitiveSelectDetect(Primitive* primitive);
#endif /* factor_hpp */

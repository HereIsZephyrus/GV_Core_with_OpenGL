//
//  initialization.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 7/14/24.
//

#ifndef initialization_hpp
#define initialization_hpp
#include <filesystem>
#include <memory>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendering.hpp"
#include "primitive.hpp"
#include "commander.hpp"
#include "camera.hpp"
#include "shape.hpp"
#include "factor.hpp"
#include "objectmodel.hpp"
#include "window.hpp"
int initImGUI(GLFWwindow *window);
int initInterect(GLFWwindow* &window);
int initStyle();
int initShaders();
#endif /* initialization_hpp */

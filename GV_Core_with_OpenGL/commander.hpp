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
#include "primitive.hpp"
#include "rendering.hpp"

enum class interectState{
    drawing,//draw feature
    toselect,//to select feature
    holding,//binded to the object
};
class Records{
public:
    static Records& getInstance(){
        static Records instance;
        return instance;
    }
    Records(const Records&) = delete;
    void operator = (const Records&) = delete;
    GLboolean keyRecord[26],pressLeft,pressRight,pressCtrl,pressShift,pressAlt;
    GLboolean dragingMode;
    interectState state;
    void initIObuffer();
private:
    Records(){}
};
class Take{
public:
    static Take& holdon(){
        static Take instance;
        return instance;
    }
    Take(const Take&) = delete;
    void operator = (const Take&) = delete;
    Primitive* obj; // for now only one primitive can be selected
    Shader* shader;
private:
    Take(){}
};
void keyBasicCommand(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseDrawCommand(GLFWwindow* window, int button, int action, int mods);
void mouseViewCommand(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
//void mouseDrawLine(GLFWwindow* window, int button, int action, int mods);

#endif /* commander_hpp */

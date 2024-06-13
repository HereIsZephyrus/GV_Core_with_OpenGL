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
    static Records& getState(){
        static Records instance;
        return instance;
    }
    Records(const Records&) = delete;
    void operator = (const Records&) = delete;
    GLboolean keyRecord[GLFW_KEY_LAST+1],pressLeft,pressRight,pressCtrl,pressShift,pressAlt;
    GLboolean dragingMode,drawingPrimitive;
    bool showCreateElementWindow;
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
    Shader* drawingShader;
    vertexArray drawingVertices;
    void addPoint();
    GLenum drawType;
    bool holdonToDraw; // hold on to draw or click to draw
private:
    Take(){}
};
void keyBasicCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseDrawCallback(GLFWwindow* window, int button, int action, int mods);
void mouseViewCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void cursorSelectCallback(GLFWwindow* window, double xpos, double ypos);
void cursorDrawCallback(GLFWwindow* window, double xpos, double ypos);
void cursorFocusCallback(GLFWwindow* window, int entered);
void windowPosCallback(GLFWwindow* window, int xpos, int ypos);

#endif /* commander_hpp */

//
//  commander_collection.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/1/24.
//

#ifndef commander_hpp
#define commander_hpp
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "primitive.hpp"
#include "rendering.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class interectState{
    drawing,//create primitive
    toselect,//to select feature
    holding,//binded to the object
    editing,//edit primitive
};
typedef std::pair<Primitive*, std::string> item;
class Records{
public:
    static Records& getState(){
        static Records instance;
        return instance;
    }
    Records(const Records&) = delete;
    void operator = (const Records&) = delete;
    GLboolean keyRecord[GLFW_KEY_LAST+1],pressLeft,pressRight,pressCtrl,pressShift,pressAlt;
    bool dragingMode,drawingPrimitive,cliping,draging;
    bool showCreateElementWindow;
    bool showAxis;
    interectState state;
    void initIObuffer();
    GLfloat previewXpos,previewYpos;
    glm::vec2 previewPosition;
    std::vector<item > primitiveList;
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
    std::vector<Primitive*> holdonObjList;
    Primitive* editingPrimitive;
    Shader* drawingShader;
    vertexArray drawingVertices;
    Shape drawType;
    bool holdonToDraw; // hold on to draw or click to draw
    pPrimitive clipShape;
    glm::mat3 transMat;
private:
    Take(){}
};

void MeauCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void keyBasicCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseDrawCallback(GLFWwindow* window, int button, int action, int mods);
void mouseViewCallback(GLFWwindow* window, int button, int action, int mods);
void mouseEditCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void cursorDefaultCallback(GLFWwindow* window, double xpos, double ypos);
void cursorDragCallback(GLFWwindow* window, double xpos, double ypos);
void cursorDrawCallback(GLFWwindow* window, double xpos, double ypos);
void cursorSelectCallback(GLFWwindow* window, double xpos, double ypos);
void cursorFocusCallback(GLFWwindow* window, int entered);
void windowPosCallback(GLFWwindow* window, int xpos, int ypos);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void keyModsToggle(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseModsToggle(GLFWwindow* window, int button, int action, int mods);
void cursorDragingDetect(GLFWwindow* window, double xpos, double ypos);
void drawModsToggle(GLFWwindow* window, int button, int action, int mods);
void viewScroll(GLFWwindow* window, double xoffset, double yoffset);
void processCursorTrace(GLFWwindow* window,double xpos, double ypos);
bool primitiveSelectDetect(Primitive* primitive);

int releaseResources(GLFWwindow* &window);
int InterectResponseCheck(GLFWwindow* &window);

#endif /* commander_hpp */

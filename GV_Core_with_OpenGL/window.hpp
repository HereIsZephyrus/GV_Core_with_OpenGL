//
//  window.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/8/24.
//

#ifndef window_hpp
#define window_hpp
#include <cstring>
#include <string>
#include <cmath>
#include <array>
#include <cstring>
#include <string>
#include <cmath>
#include <set>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "primitive.hpp"
#include "commander.hpp"

inline bool HAS_INIT_OPENGL_CONTEXT = false;
int initOpenGL(GLFWwindow *&window);
struct ItemInfo{
    int count;
    std::string typeName;
};
class WindowParas{
public:
    static WindowParas& getInstance(){
        static WindowParas instance;
        return instance;
    }
    WindowParas(const WindowParas&) = delete;
    void operator=(const WindowParas&) = delete;
    GLFWwindow * window;
    const GLint WINDOW_WIDTH = 1260; //960 + 300
    const GLint WINDOW_HEIGHT = 720;
    const GLint SIDEBAR_WIDTH = 300;
    GLint SCREEN_WIDTH,SCREEN_HEIGHT;
    GLfloat xScale,yScale;
    GLfloat defaultAlpha;
    bool mainWindowFocused;
    GLdouble lastClickTime = 0.0;
    ImVec4 backgroundColor;
    GLfloat screen2normalX(GLdouble screenX);
    GLfloat screen2normalY(GLdouble screenY);
    GLfloat normal2orthoX(GLfloat normalX);
    GLfloat normal2orthoY(GLfloat normalY);
private:
    WindowParas() {}
};
void  clipByShape();
namespace coord {
void generateCoordinateAxis();
void drawScaleText();
}

namespace gui {
constexpr GLfloat detactBias = 0.48f;
constexpr GLfloat outboundBias = 2.0f;
constexpr GLfloat borderDetectRange = 20.0f;
constexpr GLfloat menuWidth = 200.0f;
constexpr GLfloat dragCameraSpeed = 6.0f;
constexpr GLfloat doubleClickBias = 0.2f;
extern unsigned int panelStackNum; //count sidebar stack num,(not achieved)
extern float menuBarHeight;
extern interectState lastState;
extern std::array<ItemInfo, static_cast<int>(Shape::COUNT)> itemInfo;
extern std::string inputString;
extern std::set<GLuint> focusedLayers;
extern GLuint editLayer ;
extern bool isActive,toDelete;
extern std::set<pItem> readyToDelete;
void DrawGUI();
void spiltUI();
void createPrimitiveList();
void drawLayerList(Layer& parentLayer,GLuint& countLayer,bool& isActive,bool& toRearrange);
void deleteButton(std::string name,const Layer& toDeleteLayer);
void deleteButton(std::string name,pItem toDeleteItem);
std::string inputLayerName();
std::string confirmDelete();
}

#endif /* window_hpp */

//
//  painting_table.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/3/24.
//

#ifndef painting_table_hpp
#define painting_table_hpp
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
int flushTheWindow(GLFWwindow* window);
typedef std::pair<float, float>  fPoint;
namespace table {
    static std::vector<fPoint> lines;
}
#endif /* painting_table_hpp */

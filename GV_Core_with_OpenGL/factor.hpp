//
//  factor.hpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/26/24.
//

#ifndef factor_hpp
#define factor_hpp
#include <memory>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "primitive.hpp"
#include "rendering.hpp"
#include "window.hpp"

void addPoint(vertexArray& array,const GLdouble cursorX, const GLdouble cursorY);
void addPoint(vertexArray& array,const GLfloat orthoX, const GLfloat orthoY);
void addPoint(vertexArray& array,const GLfloat orthoX, const GLdouble cursorY);
void addPoint(vertexArray& array,const GLdouble cursorX, const GLfloat orthoY);
void toAlignment(vertexArray& array,Shape shape);
void editPrimitive();
void generateNewPrimitive();
void generatePreviewPrimitive(const vertexArray&);
void generateNewPrimitiveList(Shape shape,Primitive* primitive);
#endif /* factor_hpp */

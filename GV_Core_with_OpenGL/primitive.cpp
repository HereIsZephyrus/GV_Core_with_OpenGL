//
//  primitive.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 6/9/24.
//

#include <iostream>
#include <cmath>
#include "primitive.hpp"
#include "glexception.hpp"
#include "commander.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "rendering.hpp"
Primitive::Primitive(vertexArray vertices,Shape shape,GLsizei stride):stride(stride){
    if (!HAS_INIT_OPENGL_CONTEXT)
        initOpenGL(WindowParas::getInstance().window);
    m_self = this;
    this->vertices = vertices;
    this->elementList.clear();
    this->holding = false;
    //this->transMat = glm::mat4(1.0f);
    Records& record = Records::getState();
    this->layer = static_cast<GLuint>(record.primitiveList.size())+1;
    const ImVec4 uiColor = ShaderStyle::getStyle().drawColor;
    this->color = {uiColor.x,uiColor.y,uiColor.z,uiColor.w};
    switch (shape) {
        case Shape::POINTS:{
            this->shape = GL_POINTS;
            break;
        }
        case Shape::LINES:{
            this->shape = GL_LINES;
            break;
        }
        case Shape::TRIANGLE:{
            this->shape = GL_TRIANGLES;
            break;
        }
        case Shape::RECTANGLE:{
            this->shape = GL_TRIANGLE_FAN;
            break;
        }
        case Shape::CIRCLE:{ //not achieved
            this->shape = GL_TRIANGLE_STRIP;
            //this->indices = pr
            break;
        }
        case Shape::LOOP:{
            this->shape = GL_LINE_LOOP;
            break;
        }
        case Shape::POLYGEN:{
            if (ShaderStyle::getStyle().toFill)
                this->shape = GL_TRIANGLE_FAN;
            else
                this->shape = GL_LINE_LOOP;
            break;;
        }
        case Shape::CURVE:{
            this->shape = GL_LINE_STRIP;
            generateCurve();
            break;
        }
        default:{
            this->shape = GL_POINT;
            break;
        }
    }
    shader = nullptr;
    glGenVertexArrays(1,&identifier.VAO);
    glGenBuffers(1,&identifier.VBO);
    updateVertex();
}
void Primitive::rend(GLuint& program){
    const glm::vec4 color = previewStyle::color;
    GLuint colorLoc = glGetUniformLocation(program,"setColor");
    glUniform4f(colorLoc,color.x,color.y,color.z,color.w);
    
    //camera
    GLuint projectionLoc = glGetUniformLocation(program, "projection");
    GLuint viewLoc = glGetUniformLocation(program, "view");
    GLuint modelLoc = glGetUniformLocation(program, "model");
    Camera2D& camera = Camera2D::getView();
    glm::mat4 projection = camera.getProjectionMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}
void Primitive::load(){
    glBindVertexArray(identifier.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, identifier.VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof (GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
}
void Primitive::draw(){
    //std::cout<<"Draw is running"<<std::endl;
    GLint currentBuffer;
    if (shader == nullptr){
        std::cerr<<"havn't bind shader"<<std::endl;
        return;
    }
    else
        shader ->use();
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
    if (currentBuffer != identifier.VBO){
        //std::cout<<currentBuffer<<' '<<VBO<<std::endl;
        load();
    }
    glEnableVertexAttribArray(0);
    glBindVertexArray(identifier.VAO);
    //std::cout<<"drawing primitive"<<std::endl;
    rend(shader->program);
    //glDrawArrays(shape, 0,( getVertexNum()-1)*6);
    glDrawArrays(shape, 0, getVertexNum());
    // CHECK_GL_ERROR(glDrawArrays(shape, 0, vertexCount));
    glBindVertexArray(0);
    return;
}
 
void Primitive::updateVertex(){
    glBindVertexArray(identifier.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, identifier.VBO);
    //glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(transfered.size() * sizeof(GLfloat)) ,static_cast<const void*>(transfered.data()), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(GLfloat)) ,static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Primitive::transform(const glm::mat3& inputMat){
    //transfered = vertices;
    for (auto vertex = vertices.begin(); vertex != vertices.end(); vertex+=stride) {
        const GLfloat rawX = *(vertex), rawY = *(vertex+1);
        //std::cout<< inputMat[0][0]<<' '<< inputMat[1][0]<< inputMat[2][0]<<std::endl;
        //std::cout<< inputMat[0][1]<<' '<< inputMat[1][1]<< inputMat[2][1]<<std::endl;
        *(vertex) = rawX * inputMat[0][0] + rawY * inputMat[1][0] + inputMat[2][0];
        *(vertex+1) = rawX * inputMat[0][1] + rawY * inputMat[1][1] + inputMat[2][1];
    }
    updateVertex();
}
void Primitive::transform(const indexArray& vertexIndex,const glm::mat3& inputMat){
    //transfered = vertices;
    for (auto index = vertexIndex.begin(); index != vertexIndex.end(); index++) {
        const GLint beginIndex = (*index) * stride;
        const GLfloat rawX = vertices[beginIndex], rawY = vertices[beginIndex + 1];
        vertices[beginIndex] = rawX * inputMat[0][0] + rawY * inputMat[1][0] + inputMat[2][0];
        vertices[beginIndex+1] = rawX * inputMat[0][1] + rawY * inputMat[1][1] + inputMat[2][1];
    }
}
void Primitive::createOutboundElement(){
    
}
void Primitive::destroyOutboundElement(){
    
}
static GLfloat lagrange_basis(int i, GLfloat x, const std::vector<GLfloat>& points) {
    GLfloat basis = 1.0f;
    for (size_t j = 0; j < points.size(); ++j) {
        if (j != i)
            basis *= (x - points[j]) / (points[i] - points[j]);
    }
    return basis;
}
void Primitive::lagrangeInterpolation(const GLint numInterpolated,const vertexArray& controlArray,const GLsizei numControlPoints){
    const GLfloat startX = controlArray[0],endX = controlArray[(numControlPoints-1) * stride];
    std::vector<GLfloat> xVal;
    for (auto vertex = controlArray.begin(); vertex!=controlArray.end(); vertex+=stride)
        xVal.push_back(*vertex);
    for (int i = 0; i < numInterpolated; ++i) {
        GLfloat x = startX + i * (endX - startX) / (numInterpolated - 1);
        GLfloat y = 0.0f;
        for (int j = 0; j < numControlPoints; j++) {
                GLfloat basis = lagrange_basis(j, x, xVal);
                y +=controlArray[j * stride + 1] * basis;
            }
        vertices.push_back(x);vertices.push_back(y);vertices.push_back(0);
    }
}
void Primitive::splineInterpolation(const GLint numInterpolated,const vertexArray& controlArray,const GLsizei numControlPoints){
    const GLfloat startX = controlArray[0],endX = controlArray[(numControlPoints-1) * stride];
    std::vector<GLfloat> xVal,yVal;
    for (auto vertex = controlArray.begin(); vertex!=controlArray.end(); vertex+=stride){
        xVal.push_back(*vertex);
        yVal.push_back(*(vertex+1));
    }
    
    std::vector<pr::spline> splines(numControlPoints);
    std::vector<GLfloat> alpha(numControlPoints - 1);
    std::vector<GLfloat> beta(numControlPoints - 1);
    alpha[0] = 0.0f; beta[0] = 0.0f;
    for (int i = 1; i < numControlPoints - 1; i++) {
        GLfloat h_i = xVal[i] - xVal[i - 1];
        GLfloat h_i1 = xVal[i + 1] - xVal[i];
        GLfloat A = h_i;
        GLfloat B = 2.0 * (h_i + h_i1);
        GLfloat C = h_i1;
        GLfloat F = 3.0 * ((yVal[i + 1] - yVal[i]) / h_i1 - (yVal[i] - yVal[i - 1]) / h_i);
        alpha[i] = -C / (A * alpha[i - 1] + B);
        beta[i] = (F - A * beta[i - 1]) / (A * alpha[i - 1] + B);
        std::cout<<A<<' '<<B<<' '<<C<<' '<<F<<' '<<alpha[i]<<' '<<beta[i]<<std::endl;
    }

    for (int i = numControlPoints - 2; i > 0; i--) {
        splines[i].c = alpha[i] * splines[i + 1].c + beta[i];
    }

    int inTheSpline = 1; //the spline id that on
    for (int i = numControlPoints - 1; i > 0; i--) {
        double h_i = xVal[i] - xVal[i - 1];
        splines[i].a = yVal[i - 1];
        splines[i].d = (splines[i].c - splines[i - 1].c) / (6.0 * h_i);
        splines[i].b = h_i * (2.0 * splines[i].c + splines[i - 1].c) / 6.0 + (yVal[i] - yVal[i - 1]) / h_i;
        //std::cout<<"i="<<i<<','<<splines[i].a<<' '<<splines[i].b<<' '<<splines[i].c<<' '<<splines[i].d<<std::endl;
    }
    std::cout<<"start generate,total points num:"<<numInterpolated<<std::endl;
    for (int i = 0; i < numInterpolated; i++) {
        GLfloat x = startX + i * (endX - startX) / (numInterpolated - 1);
        GLfloat dx = x - xVal[inTheSpline-1];
        //std::cout<<"i="<<i<<",in the "<<inTheSpline<<','<<splines[inTheSpline].a<<' '<<splines[inTheSpline].b<<' '<<splines[inTheSpline].c<<' '<<splines[inTheSpline].d<<std::endl;
        GLfloat y = splines[inTheSpline].a + splines[inTheSpline].b  * dx + splines[inTheSpline].c * dx * dx + splines[inTheSpline].d * dx * dx * dx;
        //std::cout<<x<<' '<<y<<std::endl;
        vertices.push_back(x);vertices.push_back(y);vertices.push_back(0);
        if (abs(x - xVal[inTheSpline]) < 1.0f && inTheSpline<xVal.size()){
            std::cout<<y<<' '<<yVal[inTheSpline]<<std::endl;
            inTheSpline++;
        }
    }
    return;
}

static int binomialCoeff(int n, int k) {
    int res = 1;
    if (k > n - k) k = n - k;
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}
static glm::vec2 computeCubicBezierPoint(GLfloat t,const vertexArray& xVal,const vertexArray& yVal,const GLsizei numControlPoints) {
    GLfloat x = 0, y = 0;
    // Compute blending functions (Bernstein polynomials)
    for (int i = 0; i<numControlPoints; i++) {
        GLfloat coeff = binomialCoeff(numControlPoints, i) * pow(1 - t, numControlPoints - i) * pow(t, i);
        x += coeff * xVal[i];
        y += coeff * yVal[i];
    }
    return glm::vec2(x, y);
}
void Primitive::bezierInterpolation(const GLint numInterpolated,const vertexArray& controlArray,const GLsizei numControlPoints){
    vertexArray xVal,yVal;
    for (auto vertex = controlArray.begin(); vertex!=controlArray.end(); vertex+=stride){
        xVal.push_back(*vertex);
        yVal.push_back(*(vertex+1));
    }
        
    for (int i = 0; i < numInterpolated; ++i) {
        GLfloat t = static_cast<GLfloat>(i) / numInterpolated;
        glm::vec2 curvePoint = computeCubicBezierPoint(t,xVal,yVal,numControlPoints);
        vertices.push_back(curvePoint.x);vertices.push_back(curvePoint.y);vertices.push_back(0);
    }
    return;
}
glm::vec2 computeHermitePoint(const glm::vec2& pS, const glm::vec2& pT, const glm::vec2& t0, const glm::vec2& t1, double t) {
    const GLfloat t2 = t * t;
    const GLfloat t3 = t2 * t;
    const GLfloat h00 = 2*t3 - 3*t2 + 1;
    const GLfloat h10 = t3 - 2*t2 + t;
    const GLfloat h01 = -2*t3 + 3*t2;
    const GLfloat h11 = t3 - t2;
    const GLfloat x = h00*pS.x + h10*t0.x + h01*pT.x + h11*t1.x;
    const GLfloat y = h00*pS.y + h10*t0.y + h01*pT.y + h11*t1.y;
    return glm::vec2(x, y);
}
void Primitive::hermiteInterpolation(const GLint numInterpolated,const vertexArray& controlArray,const GLsizei numControlPoints){
    vertexArray xVal,yVal;
    for (auto vertex = controlArray.begin(); vertex!=controlArray.end(); vertex+=stride){
        xVal.push_back(*vertex);
        yVal.push_back(*(vertex+1));
    }
    const glm::vec2 pS = glm::vec2(xVal[0],yVal[0]),pT = glm::vec2(xVal[numControlPoints-1],yVal[numControlPoints-1]);
    const glm::vec2 tS = glm::vec2(xVal[1] - xVal[0],yVal[1] - yVal[0]),tT = glm::vec2(xVal[numControlPoints-2] - xVal[numControlPoints-1],yVal[numControlPoints-2] - yVal[numControlPoints-1]);
    for (int i = 0; i < numInterpolated; ++i) {
        GLfloat t = static_cast<GLfloat>(i) / numInterpolated;
        glm::vec2 curvePoint = computeHermitePoint(pS, pT, tS, tT, t);
        vertices.push_back(curvePoint.x);vertices.push_back(curvePoint.y);vertices.push_back(0);
    }
    return;
}

void Primitive::generateCurve(){
    vertexArray controlArray = vertices;
    std::cout<<controlArray.size()<<std::endl;
    const GLsizei numControlPoints = getVertexNum();
    vertices.clear();
    GLfloat length = 0;
    for (int i = 1; i < numControlPoints; i++) {
        length += std::sqrt(
                        (controlArray[i * stride] - controlArray[(i-1) * stride]) * (controlArray[i * stride] - controlArray[(i-1) * stride])+
                        (controlArray[i * stride + 1] - controlArray[(i-1) * stride + 1]) * (controlArray[i * stride + 1] - controlArray[(i-1) * stride] + 1)+
                        (controlArray[i * stride + 2] - controlArray[(i-1) * stride + 2]) * (controlArray[i * stride + 2] - controlArray[(i-1) * stride] + 2)
                        );
    }
    std::cout<<length<<std::endl;
    const GLint numInterpolated = static_cast<int>(length);
    std::cout<<"curve point num:"<<numInterpolated<<std::endl;
    //lagrangeInterpolation(numInterpolated,controlArray,numControlPoints);
    //splineInterpolation(numInterpolated, controlArray, numControlPoints);
    //bezierInterpolation(numInterpolated, controlArray, numControlPoints);
    hermiteInterpolation(numInterpolated, controlArray, numControlPoints);
}
namespace pr {
std::vector<std::unique_ptr<Primitive> >mainPrimitiveList;
pPrimitive drawPreviewPrimitive = nullptr;
pPrimitive axisPrimitive = nullptr;
}

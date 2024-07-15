//
//  initialization.cpp
//  GV_Core_with_OpenGL
//
//  Created by ChanningTong on 7/14/24.
//

#include "initialization.hpp"

int initImGUI(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    gui::spiltUI();
    Take& take = Take::holdon();
    take.drawType = Shape::NONE; //set as blank style
    gui::menuBarHeight = ImGui::GetFrameHeightWithSpacing() * WindowParas::getInstance().yScale;
    return  0;
}

int initInterect(GLFWwindow* &window){
    Records::getState().initIObuffer();
    glfwSetWindowPosCallback(window, windowPosCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetKeyCallback(window, keyBasicCallback);
    glfwSetMouseButtonCallback(window, mouseViewCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, cursorSelectCallback);
    glfwSetCursorEnterCallback(window, cursorFocusCallback);
    return 0;
}
static void checkSourceRelevantPath(){
    namespace fs = std::filesystem;
    fs::path cwd = fs::current_path();
    fs::path otherPath = "/Users/channingtong/Program/GV_Core_with_OpenGL/resources";
    fs::path relativePath = relative(otherPath, cwd);
    std::cout << "Current directory: " << cwd << std::endl;
    std::cout << "Other path: " << otherPath << std::endl;
    std::cout << "Relative path: " << relativePath << std::endl;
}

static void checkStyleBoundary() {
    GLfloat lineWidthRange[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidthRange);
    std::cout << "Supported line width range: " << lineWidthRange[0] << " to " << lineWidthRange[1] << std::endl;
    GLfloat minPointSize, maxPointSize;
    glGetFloatv(GL_POINT_SIZE_RANGE, &minPointSize);
    glGetFloatv(GL_POINT_SIZE_MAX, &maxPointSize);
    std::cout << "Supported point size range: " << minPointSize << " to " << maxPointSize<< std::endl;
}

int initStyle(){
    ShaderStyle::getStyle().initStyle();
    //init camera
    WindowParas& windowPara = WindowParas::getInstance();
    pCamera2D tempZeroCamera = pCamera2D(new CameraPara2D(glm::vec2(0.0f, 0.0f),1.0f,WindowParas::getInstance().SCREEN_WIDTH,WindowParas::getInstance().SCREEN_HEIGHT));
    cm::zeroCamera = std::move(tempZeroCamera);
    Camera2D::getView().loadSavedPara(cm::zeroCamera.get());
    //init primitive paras
    glEnable(GL_LINE_SMOOTH);
    checkStyleBoundary();
    windowPara.backgroundColor = {0.1f, 0.1f, 0.1f, 1.0f};
    initShaders();
    
    coord::generateCoordinateAxis();
    return 0;
}
int initShaders(){
    //init axis shader
    pShader axisShader (new Shader());
    axisShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
    axisShader->attchShader(rd::filePath("fillWhite.frag"),GL_FRAGMENT_SHADER);
    axisShader->linkProgram();
    rd::namedShader["axisShader"] = std::move(axisShader);
    //init preview shader
    pShader previewfillShader (new Shader());
    previewfillShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
    previewfillShader->attchShader(rd::filePath("fillWhite.frag"),GL_FRAGMENT_SHADER);
    previewfillShader->linkProgram();
    rd::namedShader["previewfillShader"] = std::move(previewfillShader);
    pShader previewCubeLineShader (new Shader());
    previewCubeLineShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
    previewCubeLineShader->attchShader(rd::filePath("cubeLine.gs"), GL_GEOMETRY_SHADER);
    previewCubeLineShader->attchShader(rd::filePath("fillWhite.frag"),GL_FRAGMENT_SHADER);
    previewCubeLineShader->linkProgram();
    rd::namedShader["previewCubeLineShader"] = std::move(previewCubeLineShader);
    pShader previewCircleLineShader (new Shader());
    previewCircleLineShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
    previewCircleLineShader->attchShader(rd::filePath("circleLine.gs"), GL_GEOMETRY_SHADER);
    previewCircleLineShader->attchShader(rd::filePath("fillWhite.frag"),GL_FRAGMENT_SHADER);
    previewCircleLineShader->linkProgram();
    rd::namedShader["previewCircleLineShader"] = std::move(previewCircleLineShader);
    pShader previewRectangleShader (new Shader());
    previewRectangleShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
    previewRectangleShader->attchShader(rd::filePath("lineRect.gs"), GL_GEOMETRY_SHADER);
    previewRectangleShader->attchShader(rd::filePath("fillWhite.frag"),GL_FRAGMENT_SHADER);
    previewRectangleShader->linkProgram();
    rd::namedShader["previewRectangleShader"] = std::move(previewRectangleShader);
    pShader previewCircleShader (new Shader());
    previewCircleShader->attchShader(rd::filePath("singleVertices.vs"),GL_VERTEX_SHADER);
    previewCircleShader->attchShader(rd::filePath("lineCircle.gs"), GL_GEOMETRY_SHADER);
    previewCircleShader->attchShader(rd::filePath("fillWhite.frag"),GL_FRAGMENT_SHADER);
    previewCircleShader->linkProgram();
    rd::namedShader["previewCircleShader"] = std::move(previewCircleShader);
    return 0;
}

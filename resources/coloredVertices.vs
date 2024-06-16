#version 410 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec4 aliasingColor;
void main() {
    gl_Position = projection * view * model * position;
    aliasingColor = color;
}

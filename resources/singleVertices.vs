#version 410 core
layout(location = 0) in vec3 position;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float thickness;
out float  xCoord;
void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    gl_PointSize = thickness;
    xCoord = gl_Position.x + gl_Position.y;
}

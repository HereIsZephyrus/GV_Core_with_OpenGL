#version 410 core
layout(location = 0) in vec3 position;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float thickness;
out float  xCoord;
void main() {
    vec4 flipPosition = vec4(-position.x,position.y,position.z,1.0);
    gl_Position = projection * view * model * flipPosition;
    gl_PointSize = thickness;
    xCoord = gl_Position.x + gl_Position.y;
}


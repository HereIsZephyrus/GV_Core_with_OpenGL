#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 250) out;

uniform float thickness;
const float PI = 3.14159;
const int resolution = 100;
void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    vec4 p1 =  gl_in[1].gl_Position;
    vec4 center = (p0+p1)/2;
    float width =  (thickness/ 800.0) / 2.0 ;
    vec2 radius = vec2( abs(center.x - p1.x),abs(center.y - p1.y));
    float dangle = - 2 * PI / resolution;
    mat2 angleMat = mat2(
                    cos(dangle), -sin(dangle),
                    sin(dangle),  cos(dangle));
    vec2 offset = vec2(1.0,0.0);
    for (int i = 0; i <= resolution; i++){
        gl_Position = center + vec4(offset * (radius - width), 0.0, 0.0);
        EmitVertex();
        gl_Position = center + vec4(offset * (radius + width), 0.0, 0.0);
        EmitVertex();
        offset = angleMat * offset;
    }
    EndPrimitive();
}

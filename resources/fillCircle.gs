
#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 200) out;

uniform float thickness;
const float PI = 3.14159;
const int resolution = 100;
void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    vec4 p1 =  gl_in[1].gl_Position;
    vec4 center = (p0+p1)/2;
    float radius = abs(center.x - p1.x);
    float dangle = - 2 * PI / resolution;
    mat2 angleMat = mat2(
                    cos(dangle), -sin(dangle),
                    sin(dangle),  cos(dangle));
    vec2 offset = vec2(radius,0.0);
    gl_Position = center;
    EmitVertex();
    for (int i = 0; i <= resolution; i++){
        gl_Position = center + vec4(offset, 0.0, 0.0);
        EmitVertex();
        if (i%2==1){
            gl_Position = center;
            EmitVertex();
        }
        offset = angleMat * offset;
    }
    gl_Position = center;
    EmitVertex();
    EndPrimitive();
}




#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 200) out;

uniform float thickness;
const float PI = 3.14159;
const int resolution = 100;
void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    vec4 p1 =  gl_in[1].gl_Position;
    float radius = distance(p0,p1);
    float dangle = - 2 * PI / resolution;
    mat2 angleMat = mat2(
                    cos(dangle), -sin(dangle),
                    sin(dangle),  cos(dangle));
    vec2 offset = vec2(0.0,0.0);
    gl_Position = p0;
    EmitVertex();
    for (int i = 0; i < resolution; i++){
        gl_Position = p0 + vec4(offset, 0.0, 0.0);
        EmitVertex();
        if (i%2==1){
            gl_Position = p0;
            EmitVertex();
        }
        offset = angleMat * offset;
    }
    EndPrimitive();
}



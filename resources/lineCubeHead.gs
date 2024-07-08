#version 410 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec4 endpoint;
uniform float thickness;

void main() {
    vec4 p =  gl_in[0].gl_Position;

    float width =  (thickness/ 800.0) / 2.0 ;
    
    endpoint = p + vec4(-width, -width, 0.0, 0.0);
    EmitVertex();

    endpoint = p - vec4(-width, width, 0.0, 0.0);
    EmitVertex();

    endpoint = p + vec4(width, width, 0.0, 0.0);
    EmitVertex();

    endpoint = p - vec4(width, -width,0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}


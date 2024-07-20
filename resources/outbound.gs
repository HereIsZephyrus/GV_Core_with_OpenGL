#version 410 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

const float lineWidth = 5.0f;
const float pointSize= 8.0f;
void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    //vec4 p1 =  gl_in[1].gl_Position;
    
    gl_Position = p0 + vec4(-1.0, -1.0, 0.0, 0.0) * pointSize;
    EmitVertex();
    gl_Position = p0 + vec4(1.0, -1.0, 0.0, 0.0) * pointSize;
    EmitVertex();
    gl_Position = p0 + vec4(-1.0, 1.0,0.0, 0.0) * pointSize;
    EmitVertex();
    gl_Position = p0 + vec4(1.0, 1.0 , 0.0, 0.0) * pointSize;
    EmitVertex();
    
    //gl_Position = p0 + vec4(1.0, 1.0,0.0, 0.0) * lineWidth;
    //EmitVertex();
    //gl_Position = p0 - vec4(1.0, 1.0,0.0, 0.0) * lineWidth;
    //EmitVertex();
    //gl_Position = p1 + vec4(1.0, 1.0,0.0, 0.0) * lineWidth;
    //EmitVertex();
    //gl_Position = p1 - vec4(1.0, 1.0,0.0, 0.0) * lineWidth;
    //EmitVertex();
    //
    //gl_Position = p1 + vec4(-1.0, -1.0, 0.0, 0.0) * pointSize;
    //EmitVertex();
    //gl_Position = p1 + vec4(-1.0, 1.0, 0.0, 0.0) * pointSize;
    //EmitVertex();
    //gl_Position = p1 + vec4(1.0, -1.0, 0.0, 0.0) * pointSize;
    //EmitVertex();
    //gl_Position = p1 + vec4(1.0, 1.0 , 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}


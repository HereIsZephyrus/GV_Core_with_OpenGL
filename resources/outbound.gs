#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 15) out;

const float lineWidth = 5.0f;
const float pointSize = 8.0f;
void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    vec4 p1 =  gl_in[1].gl_Position;
    
    gl_Position = p0 + vec4(-pointSize, -pointSize, 0.0, 0.0);
    EmitVertex();
    gl_Position = p0 + vec4(pointSize, -pointSize, 0.0, 0.0);
    EmitVertex();
    gl_Position = p0 + vec4(-pointSize, pointSize,0.0, 0.0);
    EmitVertex();
    gl_Position = p0 + vec4(pointSize, pointSize , 0.0, 0.0);
    EmitVertex();
    
    gl_Position = p0 + vec4(lineWidth, lineWidth,0.0, 0.0);
    EmitVertex();
    gl_Position = p0 - vec4(lineWidth, lineWidth,0.0, 0.0);
    EmitVertex();
    gl_Position = p1 + vec4(lineWidth, lineWidth,0.0, 0.0);
    EmitVertex();
    gl_Position = p1 - vec4(lineWidth, lineWidth,0.0, 0.0);
    EmitVertex();
    
    gl_Position = p1 + vec4(-pointSize, -pointSize, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 + vec4(-pointSize, pointSize, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 + vec4(pointSize, -pointSize, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 + vec4(pointSize, pointSize , 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}


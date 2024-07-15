#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float thickness;
void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    vec4 p1 =  gl_in[1].gl_Position;
    vec2 dp = p1.xy - p0.xy;
    gl_Position = p0 + vec4(dp.x,0.0,0.0,0.0);
    EmitVertex();
    gl_Position = p0;
    EmitVertex();
    gl_Position = p0 + vec4(dp,0.0,0.0);
    EmitVertex();
    gl_Position = p0 + vec4(0.0,dp.y,0.0,0.0);
    EmitVertex();
    EndPrimitive();
}

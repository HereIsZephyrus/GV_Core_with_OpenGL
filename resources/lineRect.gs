#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 10) out;

uniform float thickness;
void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    vec4 p1 =  gl_in[1].gl_Position;
    vec2 dig1 = vec2(0.0,0.0);
    if ((p0.x - p1.x) * (p0.y - p1.y) < 0)
        dig1 = normalize(vec2(1.0,-1.0));
    else
        dig1 = normalize(vec2(1.0,1.0));
    vec2 dig2 = vec2(-dig1.x,dig1.y);
    //vec2 dig1 = normalize(p1.xy - p0.xy);
    //vec2 dig2 = vec2(-dig1.x,dig1.y);
    float width =  (thickness/ 800.0) / 2.0 ;
    vec2 dp = p1.xy - p0.xy;
    // horizonal 1
    gl_Position = p0 + vec4(dig1 * width,0.0,0.0);
    EmitVertex();
    gl_Position = p0 - vec4(dig1 * width,0.0,0.0);
    EmitVertex();
    gl_Position = p0 + vec4(dp.x,0.0,0.0,0.0) +  vec4(dig2 * width,0.0,0.0);
    EmitVertex();
    gl_Position = p0 + vec4(dp.x,0.0,0.0,0.0) -  vec4(dig2 * width,0.0,0.0);
    EmitVertex();
    // vertical 1
    gl_Position = p0 + vec4(dp,0.0,0.0) -  vec4(dig1 * width,0.0,0.0);
    EmitVertex();
    gl_Position = p0 + vec4(dp,0.0,0.0) +  vec4(dig1 * width,0.0,0.0);
    EmitVertex();
    // horizonal 2
    gl_Position = p0 + vec4(0.0,dp.y,0.0,0.0) -  vec4(dig2 * width,0.0,0.0);
    EmitVertex();
    gl_Position = p0 + vec4(0.0,dp.y,0.0,0.0) +  vec4(dig2 * width,0.0,0.0);
    EmitVertex();
    // vertical 2
    gl_Position = p0 + vec4(dig1 * width,0.0,0.0);
    EmitVertex();
    gl_Position = p0 - vec4(dig1 * width,0.0,0.0);
    EmitVertex();
    EndPrimitive();
}

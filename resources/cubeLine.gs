#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 10) out;

uniform float thickness;

void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    vec4 p1 =  gl_in[1].gl_Position;

    vec2 direction = normalize(p1.xy - p0.xy);
    float width =  (thickness/ 800.0) / 2.0 ;
    vec2 normal = vec2(-direction.y, direction.x) * width ;

    gl_Position = p0 + vec4(-normal.x, -normal.y, 0.0, 0.0);
    EmitVertex();
    gl_Position = p0 + vec4(normal.x, -normal.y, 0.0, 0.0);
    EmitVertex();
    gl_Position = p0 + vec4(-normal.x, normal.y,0.0, 0.0);
    EmitVertex();
    gl_Position = p0 + vec4(normal.x, normal.y , 0.0, 0.0);
    EmitVertex();
    
    //gl_Position = p0 + vec4(normal, 0.0, 0.0);
    //EmitVertex();
    gl_Position = p0 - vec4(normal, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 + vec4(normal, 0.0, 0.0);
    EmitVertex();
   // gl_Position = p1 - vec4(normal, 0.0, 0.0);
    //EmitVertex();
    
    gl_Position = p1 + vec4(-normal.x, -normal.y, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 + vec4(-normal.x, normal.y, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 + vec4(normal.x, -normal.y, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 + vec4(normal.x, normal.y , 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}


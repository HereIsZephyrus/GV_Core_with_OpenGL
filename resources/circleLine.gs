#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 100) out;

uniform float thickness;
const float PI = 3.14159;
const int resolution = 20;
void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    vec4 p1 =  gl_in[1].gl_Position;

    
    vec2 direction = normalize(p1.xy - p0.xy);
    float width =  (thickness/ 800.0) / 2.0 ;
    vec2 normal = vec2(-direction.y, direction.x) * width ;

    gl_Position = p0;
    EmitVertex();
    float dangle = - PI / resolution;
    mat2 angleMat = mat2(
                    cos(dangle), -sin(dangle),
                    sin(dangle),  cos(dangle));
    vec2 offset = normal;
    for (int i = 0; i < resolution; i++){
        gl_Position = p0 + vec4(offset, 0.0, 0.0);
        EmitVertex();
        if (i%2==1){
            gl_Position = p0;
            EmitVertex();
        }
        offset = angleMat * offset;
    }
    gl_Position = p0 + vec4(offset, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = p0 + vec4(normal, 0.0, 0.0);
    EmitVertex();
    gl_Position = p0 - vec4(normal, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 + vec4(normal, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 - vec4(normal, 0.0, 0.0);
    EmitVertex();
    
    gl_Position = p1;
    EmitVertex();
    for (int i = 0; i < resolution; i++){
        gl_Position = p1 + vec4(offset, 0.0, 0.0);
        EmitVertex();
        if (i%2==1){
            gl_Position = p1;
            EmitVertex();
        }
        offset = angleMat * offset;
    }
    gl_Position = p1 + vec4(offset, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}



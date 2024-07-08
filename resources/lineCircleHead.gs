#version 410 core

layout(points) in;
layout(triangle_fan, max_vertices = 4) out;

uniform float thickness;
out vec4 endpoint;
void main() {
    vec4 p =  gl_in[0].gl_Position;

    //vec2 direction = normalize(p1.xy - p0.xy);
    float radius = (thickness/ 800.0) / 2.0 ;

    //endpoint = p;
    endpoint = p + vec4(normal, 0.0, 0.0);
    EmitVertex();

    endpoint = p - vec4(normal, 0.0, 0.0);
    EmitVertex();

    endpoint = p + vec4(normal, 0.0, 0.0);
    EmitVertex();

    endpoint = p - vec4(normal, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}


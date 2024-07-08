#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 endpoint[];
out vec4 linePosition;
uniform float thickness;

void main() {
    vec4 p0 =  gl_in[0].gl_Position;
    vec4 p1 =  gl_in[1].gl_Position;

    vec2 direction = normalize(p1.xy - p0.xy);
    vec2 normal = vec2(-direction.y, direction.x) * (thickness/ 800.0) / 2.0 ;

    linePosition = p0 + vec4(normal, 0.0, 0.0);
    EmitVertex();

    linePosition = p0 - vec4(normal, 0.0, 0.0);
    EmitVertex();

    linePosition = p1 + vec4(normal, 0.0, 0.0);
    EmitVertex();

    linePosition = p1 - vec4(normal, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}


#version 410 core

layout(location = 0) in vec4 vertex;

uniform vec2  resolution;
uniform float thickness;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main(){
    int line_i = gl_VertexID / 6;
    int tri_i  = gl_VertexID % 6;

    vec4 va[4];
    for (int i=0; i<4; ++i){
        va[i].xyz /= va[i].w;
        va[i].xy = (va[i].xy + 1.0) * 0.5 * resolution;
    }

    vec2 v_line  = normalize(va[2].xy - va[1].xy);
    vec2 nv_line = vec2(-v_line.y, v_line.x);

    vec4 pos;
    if (tri_i == 0 || tri_i == 1 || tri_i == 3){
        vec2 v_pred  = normalize(va[1].xy - va[0].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_pred.y, v_pred.x));

        pos = va[1];
        pos.xy += v_miter * thickness * (tri_i == 1 ? -0.5 : 0.5) / dot(v_miter, nv_line);
    }
    else{
        vec2 v_succ  = normalize(va[3].xy - va[2].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_succ.y, v_succ.x));

        pos = va[2];
        pos.xy += v_miter * thickness * (tri_i == 5 ? 0.5 : -0.5) / dot(v_miter, nv_line);
    }

    pos.xy = pos.xy / resolution * 2.0 - 1.0;
    pos.xyz *= pos.w;
    gl_Position = projection * view * model * pos;
}

#version 410 core
layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 124) out;

//uniform int Segments;
uniform float thickness;
const int SegmentsMax = 30;
const int SegmentsMin = 3;

vec4 toBezier(float delta, int i, vec4 P0, vec4 P1, vec4 P2, vec4 P3){
    float t = delta * float(i);
    float t2 = t * t;
    float one_minus_t = 1.0 - t;
    float one_minus_t2 = one_minus_t * one_minus_t;
    return (P0 * one_minus_t2 * one_minus_t + P1 * 3.0 * t * one_minus_t2 + P2 * 3.0 * t2 * one_minus_t + P3 * t2 * t);
}

void main(){
    int Segments =20;
    int nSegments = (Segments > SegmentsMax)? SegmentsMax : Segments;
    nSegments = (nSegments < SegmentsMin)? SegmentsMin: nSegments;
    int numSegments = 100;
    float t = 0.0;
    float dt = 1.0 / (numSegments - 1);
    vec4 B[4];
    B[0] = gl_in[0].gl_Position;
    B[1] = gl_in[1].gl_Position;
    B[2] = gl_in[2].gl_Position;
    B[3] = gl_in[3].gl_Position;
    float delta = 1.0 / float(nSegments);
    vec4 Points[4];
    for (int i=0; i<=nSegments; ++i){
        if (i==0){
            Points[1] = toBezier(delta, i, B[0], B[1], B[2], B[3]);
            Points[2] = toBezier(delta, i+1, B[0], B[1], B[2], B[3]);
            Points[3] = toBezier(delta, i+2, B[0], B[1], B[2], B[3]);
            vec4 dir = normalize(Points[1] - Points[2]);
            Points[0] = Points[1] + dir * 0.01;
        }
        else if (i < nSegments-1){
            Points[0] = Points[1];
            Points[1] = Points[2];
            Points[2] = Points[3];
            Points[3] = toBezier(delta, i+2, B[0], B[1], B[2], B[3]);
        }
        else {
            Points[0] = Points[1];
            Points[1] = Points[2];
            Points[2] = Points[3];
            vec4 dir = normalize(Points[2] - Points[1]);
            Points[3] = Points[2] + dir * 0.01;
        }
        for (int k = 0; k<4; k++){
            gl_Position = Points[k];
            EmitVertex();
        }
    }
    EndPrimitive();
}

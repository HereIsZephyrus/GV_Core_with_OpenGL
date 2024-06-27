#version 410 core
layout(location = 0) in vec3 inPosition;
uniform float time;
uniform vec3 controlPoints[10];
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    vec3 P = vec3(0.0);
    for (int i = 0; i < numControlPoints; ++i) {
        vec3 Pi = controlPoints[i];
        float Li = 1.0;
        for (int j = 0; j < numControlPoints; ++j) {
            if (i != j) {
                Li *= (time - float(j)) / (float(i) - float(j));
            }
        }
        P += Pi * Li;
    }
    gl_Position = projection * view * model * vec4(P , 1.0);
}

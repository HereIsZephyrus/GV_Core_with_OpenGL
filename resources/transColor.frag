#version 410 core
in vec4 aliasingColor;
out vec4 color;
void main( ){
    color = aliasingColor;
}

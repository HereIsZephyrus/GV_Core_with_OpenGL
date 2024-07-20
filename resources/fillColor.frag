#version 410 core
uniform vec4 setColor;
uniform float brightness = 1.0f;
out vec4 color;
void main( ){
    color = vec4(setColor.xyz * brightness,setColor.w);
}

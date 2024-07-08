#version 410 core

in float xCoord;
uniform vec4 setColor;
out vec4 color;

void main() {
    const float lineLength = 1/50.0f;
    const float gapLength = 1/100.0f;
    float t = mod(xCoord, lineLength + gapLength);
    if (t > lineLength) {
        discard;
    }
    color = setColor;
}


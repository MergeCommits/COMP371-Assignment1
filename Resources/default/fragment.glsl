#version 330 core

in vec4 fsColor;

uniform mat4 da;

out vec4 outColor;

void main() {
    outColor = fsColor;
    outColor = vec4(da[0][0]-0.5f, da[1][1]-0.5f, da[2][2]-0.5f, da[3][3]-0.5f);
}

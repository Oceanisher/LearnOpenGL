#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;

out vec3 outColor;
out vec2 textCoor;

void main()
{
    gl_Position = vec4(vPos, 1.0);
    outColor = vColor;
    textCoor = vec2(vTexture.x, vTexture.y);
}

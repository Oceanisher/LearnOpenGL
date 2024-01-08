#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;

out vec3 outColor;
out vec2 textCoor;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vPos, 1.0);
    outColor = vColor;
    textCoor = vTexture;
}

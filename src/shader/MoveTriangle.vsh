#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

out vec3 outColor;

//随机位移
uniform vec3 randomPos;

void main()
{
    gl_Position = vec4(vPos + randomPos, 1.0);
    outColor = vColor;
}

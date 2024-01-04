#version 460 core
layout (location = 0) in vec3 vPos;

//随机位移
uniform vec3 randomPos;

void main()
{
    gl_Position = vec4(vPos + randomPos, 1.0);
}

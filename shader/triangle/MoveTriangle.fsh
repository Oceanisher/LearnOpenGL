#version 460 core
out vec4 fragColor;

//随机颜色
uniform vec4 randomColor;

void main()
{
    fragColor = randomColor;
}
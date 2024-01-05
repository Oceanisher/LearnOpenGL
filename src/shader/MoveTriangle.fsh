#version 330 core
in vec3 outColor;
out vec4 fragColor;

//随机颜色
uniform vec3 randomColor;

void main()
{
    vec3 color = randomColor + outColor;
    fragColor = vec4(color, 1.0);
}
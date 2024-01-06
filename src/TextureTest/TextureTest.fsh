#version 330 core
in vec3 outColor;
in vec2 textCoor;

out vec4 fragColor;

//纹理
uniform sampler2D texture1;

void main()
{
    fragColor = texture(texture1, textCoor);
}
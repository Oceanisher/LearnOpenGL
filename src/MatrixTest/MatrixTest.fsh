#version 330 core
in vec3 outColor;
in vec2 textCoor;

out vec4 fragColor;

//纹理
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    fragColor = mix(texture(texture1, textCoor), texture(texture2, textCoor), 0.2);
}
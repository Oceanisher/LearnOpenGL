#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexture;
layout (location = 2) in vec3 vNormal;

out vec2 textCoor;
out vec3 normal;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    //输出顶点的世界坐标，只需要用到模型矩阵
    worldPos = vec3(model * vec4(vPos, 1.0));
    textCoor = vTexture;
    //法线数据，需要根据模型当前的旋转、缩放等来变更；所以用模型矩阵的左上角3x3部分的逆矩阵的转置矩阵来处理、并最后转成单位向量
    normal = normalize(mat3(transpose(inverse(model))) * vNormal);
}
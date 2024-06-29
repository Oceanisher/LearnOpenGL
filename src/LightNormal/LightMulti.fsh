#version 330 core
out vec4 FragColor;

//物体材质
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

//平行光
struct LightDirectional
{
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//点光源
struct LightPoint
{
    vec3 pos;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//聚光
struct LightSpot
{
    vec3 pos;
    vec3 dir;
    float innerCutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//这里光源数组的数量设置了多少，代码中就一定要赋值多少个光源，否则会导致全黑
#define NR_POINT_LIGHTS 2
#define NR_SPOT_LIGHTS 2

in vec2 textCoor;
in vec3 normal;
in vec3 worldPos;

//摄像机位置
uniform vec3 camPos;
uniform Material material;
uniform LightDirectional dirLight;
uniform LightPoint pointLights[NR_POINT_LIGHTS];
uniform LightSpot spotLights[NR_SPOT_LIGHTS];

//平行光计算
vec3 CalLightDir(LightDirectional light, vec3 normal, vec3 diffuseOriginColor, vec3 specularOriginColor, vec3 viewDir);
//点光源计算
vec3 CalLightPoint(LightPoint light, vec3 fragPos, vec3 normal, vec3 diffuseOriginColor, vec3 specularOriginColor, vec3 viewDir);
//聚光计算
vec3 CalLightSpot(LightSpot light, vec3 fragPos, vec3 normal, vec3 diffuseOriginColor, vec3 specularOriginColor, vec3 viewDir);

void main()
{
    //漫反射原始颜色
    vec3 diffuseColor = texture(material.diffuse, textCoor).rgb;
    //镜面反射原始颜色
    vec3 specularColor = texture(material.specular, textCoor).rgb;
    //视线的反向
    vec3 viewDir = normalize(camPos - worldPos);
    
    vec3 finalColor = vec3(0.0f);
    //平行光
    finalColor = CalLightDir(dirLight, normal, diffuseColor, specularColor, viewDir);

    //点光源
    for (int i = 0 ; i < NR_POINT_LIGHTS; i++)
    {
        finalColor += CalLightPoint(pointLights[i], worldPos, normal, diffuseColor, specularColor, viewDir);
    }
    //聚光
    for (int j = 0 ; j < NR_SPOT_LIGHTS; j++)
    {
        finalColor += CalLightSpot(spotLights[j], worldPos, normal, diffuseColor, specularColor, viewDir);
    }

    FragColor = vec4(finalColor, 1.0F);
}

//平行光计算
vec3 CalLightDir(LightDirectional light, vec3 normal, vec3 diffuseOriginColor, vec3 specularOriginColor, vec3 viewDir)
{
    //光源入射向量的反向
    vec3 inVec = normalize(-light.dir);

    //环境光
    vec3 ambientColor = light.ambient * diffuseOriginColor;
    
    //漫反射，使用法线、入射光的反向向量的点乘值，来算作漫反射系数；由于负值无效，所以要max一下
    float cosVal = max(dot(normal, inVec), 0.0F);
    vec3 normalColor = cosVal * light.diffuse * diffuseOriginColor;

    //镜面反射，镜面强度0.5、反光度32
    //reflect可以计算出反射向量，输入入射光、法线
    vec3 viewReflectVec = reflect(-inVec, normal);
    //观察向量的反向、反射光的点乘值，然后再32次幂一下；这个幂值就是反光度
    float viewVal = pow(max(dot(viewDir, viewReflectVec), 0.0F), material.shininess);
    vec3 specularColor = viewVal * light.specular * specularOriginColor;

    return ambientColor + normalColor + specularColor;
}

//点光源计算
vec3 CalLightPoint(LightPoint light, vec3 fragPos, vec3 normal, vec3 diffuseOriginColor, vec3 specularOriginColor, vec3 viewDir)
{
    //计算距离
    float distance = length(fragPos - light.pos);
    //计算衰减
    float decline = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    //光源入射向量的反向
    vec3 inVec = normalize(light.pos - fragPos);

    //环境光
    vec3 ambientColor = light.ambient * diffuseOriginColor;

    //漫反射，使用法线、入射光的反向向量的点乘值，来算作漫反射系数；由于负值无效，所以要max一下
    float cosVal = max(dot(normal, inVec), 0.0F);
    vec3 normalColor = cosVal * light.diffuse * diffuseOriginColor;

    //镜面反射，镜面强度0.5、反光度32
    //reflect可以计算出反射向量，输入入射光、法线
    vec3 viewReflectVec = reflect(-inVec, normal);
    //观察向量的反向、反射光的点乘值，然后再32次幂一下；这个幂值就是反光度
    float viewVal = pow(max(dot(viewDir, viewReflectVec), 0.0F), material.shininess);
    vec3 specularColor = viewVal * light.specular * specularOriginColor;

    return decline * (ambientColor + normalColor + specularColor);
}

//聚光计算
vec3 CalLightSpot(LightSpot light, vec3 fragPos, vec3 normal, vec3 diffuseOriginColor, vec3 specularOriginColor, vec3 viewDir)
{
    //计算入射向量的反向
    vec3 inVec = normalize(light.pos - fragPos);
    //计算入射与光方向的余弦值
    float dynamicVal = dot(-inVec, normalize(light.dir));
    //计算边缘模糊
    float attenuation = clamp((dynamicVal - light.outerCutOff) / (light.innerCutOff - light.outerCutOff), 0.0f, 1.0f);

    //计算距离
    float distance = length(fragPos - light.pos);
    //计算衰减
    float decline = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //环境光
    vec3 ambientColor = light.ambient * diffuseOriginColor;
    
    //漫反射，使用法线、入射光的反向向量的点乘值，来算作漫反射系数；由于负值无效，所以要max一下
    float cosVal = max(dot(normal, inVec), 0.0F);
    vec3 normalColor = cosVal * light.diffuse * diffuseOriginColor;

    //镜面反射，镜面强度0.5、反光度32
    //reflect可以计算出反射向量，输入入射光、法线
    vec3 viewReflectVec = reflect(-inVec, normal);
    //观察向量的反向、反射光的点乘值，然后再32次幂一下；这个幂值就是反光度
    float viewVal = pow(max(dot(viewDir, viewReflectVec), 0.0F), material.shininess);
    vec3 specularColor = viewVal * light.specular * specularOriginColor;

    return decline * attenuation * (ambientColor + normalColor + specularColor);
}
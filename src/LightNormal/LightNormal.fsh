#version 330 core
in vec2 textCoor;
in vec3 normal;
in vec3 worldPos;
out vec4 FragColor;

//摄像机位置
uniform vec3 camPos;

//设置物体材质
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

//设置光照属性
struct Light {
    //光源位置
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    //光源入射向量的反向
    vec3 inVec = normalize(light.position - worldPos);
    
    //环境光
    vec3 ambientColor = light.ambient * texture(material.diffuse, textCoor).rgb;
    
    //漫反射，使用法线、入射光的反向向量的点乘值，来算作漫反射系数；由于负值无效，所以要max一下
    float cosVal = max(dot(normal, inVec), 0.0F);
    vec3 normalColor = cosVal * light.diffuse * texture(material.diffuse, textCoor).rgb;
    
    //镜面反射，镜面强度0.5、反光度32
    vec3 viewOutVec = normalize(camPos - worldPos);
    //reflect可以计算出反射向量，输入入射光、法线
    vec3 viewReflectVec = reflect(-inVec, normal);
    //观察向量的反向、反射光的点乘值，然后再32次幂一下；这个幂值就是反光度
    float viewVal = pow(max(dot(viewOutVec, viewReflectVec), 0.0F), material.shininess);
    vec3 specularColor = viewVal * light.specular * texture(material.specular, textCoor).rgb;
    
    FragColor = vec4(ambientColor + normalColor + specularColor, 1.0F);
}
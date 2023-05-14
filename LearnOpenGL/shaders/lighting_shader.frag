#version 330 core

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 direction;
    vec3 position;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

#define NR_POINT_LIGHTS 1

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight torchLight;
uniform Material material;
uniform vec3 viewPos;


vec3 diffuseVec3 = vec3(texture(material.texture_diffuse0, texCoord));
vec3 specularVec3 = vec3(texture(material.texture_diffuse0, texCoord));

void main()
{

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calcDirLight(dirLight, norm, viewDir);
    
//    for(int i = 0; i < NR_POINT_LIGHTS; ++i) {
//        result += calcPointLight(pointLights[i], norm, viewDir);
//    }

    result += calcSpotLight(torchLight, norm, viewDir);

    fragColor = vec4(result, 1.0f);
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);
    
    float diffuseFactor = max(dot(normal, lightDir), 0.0f);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);


    vec3 ambient = light.ambient * diffuseVec3;
    vec3 diffuse = light.diffuse * diffuseFactor * diffuseVec3;
    vec3 specular = light.specular * specularFactor * specularVec3;


    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diffuseFactor = max(dot(normal, lightDir), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + (light.linear * lightDistance) + (light.quadratic * lightDistance * lightDistance));

    vec3 ambient = light.ambient * diffuseVec3;
    vec3 diffuse = light.diffuse * diffuseFactor * diffuseVec3;
    vec3 specular = light.specular * specularFactor * specularVec3;

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diffuseFactor = max(dot(normal, lightDir), 0.0f);

    float theta = dot(-lightDir, normalize(light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = smoothstep(0.0f, 1.0f, (theta - light.outerCutOff) / epsilon);
    
    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + (light.linear * lightDistance) + (light.quadratic * lightDistance * lightDistance));

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    vec3 ambient = light.ambient * diffuseVec3;
    vec3 diffuse = light.diffuse * diffuseFactor * diffuseVec3;
    vec3 specular = light.specular * specularFactor * specularVec3;

    return (ambient + diffuse + specular) * intensity * attenuation;
}
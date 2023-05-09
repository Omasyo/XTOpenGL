#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

#define NR_POINT_LIGHTS 4

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPos;

void main()
{

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calcDirLight(dirLight, norm, viewDir);
    
    for(int i = 0; i < NR_POINT_LIGHTS; ++i) {
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
    }

//    float theta = dot(lightDir, normalize(-light.direction));
//    if(theta > light.outerCutOff) 
//    {
//        float epsilon = light.cutOff - light.outerCutOff;
//        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
////        float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff) / epsilon);
//
//        float diff = max(dot(norm, lightDir), 0.0f);
//        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord)) * intensity;
//
//        vec3 viewDir = normalize(viewPos - fragPos);
//        vec3 reflectDir = reflect(-lightDir, norm);
//
//        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
//        vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord)) * intensity;  
//
//        fragColor = vec4(diffuse + specular, 1.0f) * attenuation;
//    }

    fragColor = vec4(result, 1.0f);
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);
    
    float diffuseFactor = max(dot(normal, lightDir), 0.0f);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);


    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.specular * specularFactor * vec3(texture(material.specular, texCoord));


    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diffuseFactor = max(dot(normal, lightDir), 0.0f);

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + (light.linear * lightDistance) + (light.quadratic * lightDistance * lightDistance));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord)) * attenuation;
    vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.diffuse, texCoord)) * attenuation;
    vec3 specular = light.specular * specularFactor * vec3(texture(material.specular, texCoord)) * attenuation;

    return (ambient + diffuse + specular);
}
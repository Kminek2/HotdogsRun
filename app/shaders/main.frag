#version 450

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
	vec3 pos;
    vec3 col;
	vec3 functions;
};

struct SpotLight {
	vec3 pos;
    vec3 col;
	vec3 functions;

	vec3 direction;
	vec2 cutOffs;
};


layout(constant_id = 0) const int MAX_TEXTURES = 64;

layout(push_constant, std430) uniform dirLightAndViewPos{
    vec3 lDirection;
	
    vec3 lAmbient;
    vec3 lDiffuse;
    vec3 lSpecular;

    vec3 cPos;

    uint pLNum;
    uint sLNum;
};

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragPos;

layout(set = 0, binding = 1) uniform sampler2D samplers;
layout(std430, binding = 2) buffer PointLightBuffer {
    readonly PointLight pointLights[];
};
layout(std430, binding = 3) buffer SpotLightBuffer {
    readonly SpotLight spotLights[];
};


layout(location = 0) out vec4 outColor;



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 pos, vec3 viewDir);

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(cPos - fragPos);
    vec3 result = vec3(0);
    vec3 texColor = texture(samplers, vec2(fragTexCoord.x, fragTexCoord.y / MAX_TEXTURES)).xyz;

    DirLight dirLight;
    dirLight.direction = lDirection;
    dirLight.ambient = lAmbient;
    dirLight.diffuse = lDiffuse;
    dirLight.specular = lSpecular;

    if(dirLight.direction != vec3(0, 0, 0))
    {
        result += CalcDirLight(dirLight, norm, viewDir);
    }else{
        result += texColor;
    }

    /*for(int i = 0; i < pLNum; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
    }

    for(int i = 0; i < sLNum; i++)
    {
        result += CalcSpotLight(spotLights[i], norm, fragPos, viewDir);
    }*/

    outColor = vec4(result * texColor, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir){
    vec3 lightDir = normalize(light.pos - pos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    // attenuation
    float distance = length(light.pos - pos);
    float attenuation = 1.0 / (light.functions.x + light.functions.y * distance + light.functions.z * (distance * distance));    
    // combine results
    vec3 ambient = light.col * 0.05;
    vec3 diffuse = light.col * 0.8 * diff;
    vec3 specular = light.col * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 pos, vec3 viewDir){
    vec3 lightDir = normalize(light.pos - pos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    // attenuation
    float distance = length(light.pos - pos);
    float attenuation = 1.0 / (light.functions.x + light.functions.y * distance + light.functions.z * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOffs.x - light.cutOffs.y;
    float intensity = clamp((theta - light.cutOffs.y) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.col * 0.05;
    vec3 diffuse = light.col * diff;
    vec3 specular = light.col * spec;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);

}
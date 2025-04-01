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
	vec2 cutOffs;
	vec3 pos;
    vec3 col;
	vec3 functions;

	vec3 direction;
};

struct ColorChanges{
    vec3 from;
    vec3 to;
};

struct InstColorChange{
    uint index;
    uint amount;
};

layout(constant_id = 0) const int MAX_TEXTURES = 64;

layout(push_constant, std430) uniform dirLightAndViewPos{
    vec4 lDirection;
	
    vec4 lAmbient;
    vec4 lDiffuse;
    vec4 lSpecular;

    vec4 cPos;

    uint pLNum;
    uint sLNum;
};

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragPos;
layout(location = 3) flat in InstColorChange colorChange;
//layout(location = 5) in vec4 lightSpacePos;

layout(set = 0, binding = 1) uniform sampler2D samplers;
//layout(set = 0, binding = 5) uniform sampler2D shadowMap;
layout(std430, binding = 2) buffer PointLightBuffer {
    readonly PointLight pointLights[];
};
layout(std430, binding = 3) buffer SpotLightBuffer {
    readonly SpotLight spotLights[];
};

layout(std430, binding = 4) buffer ColorChangesBuffer {
    readonly ColorChanges colorChanges[];
};

layout(location = 0) out vec4 outColor;



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir, vec3 col);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 pos, vec3 viewDir, vec3 col);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);


float gamma = 2.2;


void main() {
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(cPos.xyz - fragPos);
    vec3 result = vec3(0, 0, 0);
    vec3 texColor = texture(samplers, vec2(fragTexCoord.x, fragTexCoord.y / MAX_TEXTURES)).xyz;
    vec3 resColor = texColor;

    for(int i = 0; i < colorChange.amount; i++)
        //if(length(normalize(texColor) - normalize(colorChanges[colorChange.index + i].from)) <= 0.2 && abs(length(texColor) - length(colorChanges[colorChange.index + i].from)) <= 0.4)
        if(distance(texColor, pow(colorChanges[colorChange.index + i].from, vec3(gamma))) <= 0.04)
            resColor = colorChanges[colorChange.index + i].to;


    //resColor = pow(resColor, vec3(gamma));
    DirLight dirLight;
    dirLight.direction = lDirection.xyz;
    dirLight.ambient = lAmbient.xyz;
    dirLight.diffuse = lDiffuse.xyz;
    dirLight.specular = lSpecular.xyz;

    if(dirLight.direction != vec3(0, 0, 0))
    {
        result += CalcDirLight(dirLight, norm, viewDir);
    }else{
        result += vec3(0.5, 0.5, 0.5);
    }

    for(int i = 0; i < pLNum; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir, resColor);
    }

    for(int i = 0; i < sLNum; i++)
    {
        result += CalcSpotLight(spotLights[i], norm, fragPos, viewDir, resColor);
    }

    outColor = vec4(result * resColor, 1.0);
    //outColor.rgb = pow(outColor.rgb, vec3(1.0/gamma));
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

    //float shadow = ShadowCalculation(lightSpacePos, normal, lightDir);       
    vec3 lighting = (ambient + diffuse + specular);
    return (lighting);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir, vec3 col){
    vec3 lightDir = normalize(light.pos - pos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    if(diff == 0 && length(light.pos - pos) <= 0.2 && length(normalize(col) - normalize(light.col)) <= 0.2)
        return light.col * 100;
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 2);
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

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 pos, vec3 viewDir, vec3 col){
    vec3 lightDir = normalize(light.pos - pos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    if(diff == 0 && length(light.pos - pos) <= 0.2 && length(normalize(col) - normalize(light.col)) <= 0.2)
        return light.col * 100;
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 2);
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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

//    float closestDepth = texture(shadowMap, projCoords.xy).r;  
    float currentDepth = projCoords.z;  
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.2f);
//    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

//    if(projCoords.z > 1.0)
//        shadow = 0.0;


//    return shadow;
      return 0.0;
}
#version 330 core
in vec3 normal;
in vec3 position;
in vec2 texCoord;
out vec4 fragColor;

uniform vec3 viewPos;//보고있는 눈의 위치

struct Light {
    vec3 position;
    vec3 attenuation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
 
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

void main() {
    vec3 texColor = texture2D(material.diffuse, texCoord).xyz;
    vec3 ambient = texColor * light.ambient;

    float dist = length(light.position - position);
    vec3 distPoly = vec3(1.0, dist, dist*dist); // 1.0, d, d*d
    // distPoly.x * light.attenuation.x +
    // distPoly.y * light.attenuation.y +
    // distPoly.z * light.attenuation.z 
    // ==>
    // 1 * light.attenuation.x +
    // d * light.attenuation.y +
    // d*d * light.attenuation.z 
    // ==>
    // 1 * Kc +
    // d * Kl +
    // d*d * Kq 
    // ==>
    // 1 * Kc + d * Kl + d*d * Kq  
    float attenuation = 1.0 / dot(distPoly, light.attenuation);
    vec3 lightDir = (light.position - position) / dist;
    vec3 pixelNorm = normalize(normal);
    float diff = max(dot(pixelNorm, lightDir), 0.0);
    vec3 diffuse = diff * texColor * light.diffuse;

    vec3 specColor = texture2D(material.specular, texCoord).xyz;
    vec3 viewDir = normalize(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, pixelNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); //pow 지수함수 반사광의 영역 크기를 조절
    vec3 specular = spec * specColor * light.specular;

    vec3 result = (ambient + diffuse + specular) * attenuation;

    fragColor = vec4(result, 1.0); 
}
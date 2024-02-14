#version 330 core
in vec3 normal;
in vec3 position;
in vec2 texCoord;
out vec4 fragColor;

uniform vec3 lightPos;//vertex pos 과의 차로 빛의 방향을 계산
uniform vec3 lightColor; //광원컬러
uniform vec3 objectColor;//오브젝트 컬러
uniform float ambientStrength;
uniform float specularStrenght;
uniform float specularShininess;
uniform vec3 viewPos;//보고있는 눈의 위치

void main() {
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - position);
    vec3 pixelNorm = normalize(normal);
    vec3 diffuse = max(dot(pixelNorm, lightDir), 0.0) * lightColor;

    vec3 viewDir = normalize(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, pixelNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess); //pow 지수함수 반사광의 영역 크기를 조절
    vec3 specular = specularStrenght * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    fragColor = vec4(result, 1.0);
}
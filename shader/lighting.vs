#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 transform;

out vec3 nomal;
out vec2 texCoord;

void main() {
    gl_Position = transform * vec4(aPos, 1.0);
    nomal = aNormal;
    texCoord = aTexCoord;
}
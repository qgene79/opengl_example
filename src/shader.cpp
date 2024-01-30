#include "shader.h"

ShaderUPtr Shader::CreateFromFile(cost std::string& filename, GLenum shaderType) {
    auto shader = std::unique_ptr<Shader>(new Shader());
    if (!shader->LoadFile(filename, shaderType))
        return nullptr;
    return std::move(shader);
}
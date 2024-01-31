#include "context.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
    return nullptr;
    return std::move(context);
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_LINE_STRIP, 0, 4);
}

bool Context::Init() {

    float vertices[] = {
        0.5f, 0.5f, 0.0f, // top right 0 
        0.5f, -0.5f, 0.0f, // bottom right 1
        -0.5f, -0.5f, 0.0f, // bottom left 2
        -0.5f, 0.5f, 0.0f, // top left 3
        };
    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    //vertex array object(vao) 생성 (vertex 버퍼 전에 선언)
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    //vertex buffer object(vbo) 생성
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 12);

    //vertex attribute array setting
    glEnableVertexAttribArray(0);// 0번 어트리뷰트를 사용 할꺼고 (simple.vs 의 location = 0)
    //floating pointer 값이 3개, GL_FALSE 노말라이즈 할필요 없음, sizeof(float) * 3 스트라이드: 다음 점에서의 어트르뷰트는 float * 3 개 
    //결국 위 어레이 값을 0번째 부터 3개씩 float 값으로 쪼개 simple.vs 에 aPos vec3(vector 3 형태로)에 넣음
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    //index buffer
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    //전체 Clear 할 색 지정
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    return true;
}

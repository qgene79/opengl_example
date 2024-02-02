#include "context.h"
#include "image.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
    return nullptr;
    return std::move(context);
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // static float time = 0.0f;
    // float t = sinf(time) * 0.5f + 0.5f;
    // auto loc = glGetUniformLocation(m_program->Get(), "color");
    m_program->Use();
    // glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // time += 0.0016f;
}

bool Context::Init() {

    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        };
    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    //vertex array object(vao) 생성 (vertex 버퍼 전에 선언)
    m_vertexLayout = VertexLayout::Create();

    //vertex buffer object(vbo) 생성
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);

    //vertex attribute array setting
    //floating pointer 값이 3개, GL_FALSE 노말라이즈 할필요 없음, sizeof(float) * 3 스트라이드: 다음 점에서의 어트르뷰트는 float * 3 개 
    //0번 어트리뷰트를 사용 할꺼고 (simple.vs 의 location = 0)
    //결국 위 어레이 값을 0번째 부터 3개씩 float 값으로 쪼개 simple.vs 에 aPos vec3(vector 3 형태로)에 넣음
    //m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    //postion
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    //rgb
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    //texture coordinate
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

    //index buffer
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    //loading shader
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    //program 인스턴스 생성, shader를 링크한 프로그램 생성
    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    //uniform 값 입력 추가 
    // auto loc = glGetUniformLocation(m_program->Get(), "color");//fs의 color
    // m_program->Use();
    // glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f);

    //전체 Clear 할 색 지정
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);

    //cpu memory 할당
    auto image = Image::Load("./images/container.jpg");
    if (!image) {
        return false;
    }
    SPDLOG_INFO("image {}x{}, {} channels",
        image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    //gpu memory 할당
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
        image->GetWidth(), image->GetHeight(), 0, 
        GL_RGB, GL_UNSIGNED_BYTE, image->GetData());

    return true;
}

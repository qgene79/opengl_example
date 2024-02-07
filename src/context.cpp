#include "context.h"
#include "image.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
    return nullptr;
    return std::move(context);
}

void Context::Render() {
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_program->Use();
    auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 30.0f); //0.01 ~ 30.0 의 공간//너무크게 넓히면 안좋아

//camera
    //auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    float angle = glfwGetTime() * glm::pi<float>() * 0.5f; //4초마다 
    auto x = sinf(angle) * 10.0f;//거리 10
    auto z = cosf(angle) * 10.0f;//거리 10
    auto cameraPos = glm::vec3(x, 0.0f, z);
    auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    auto view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    // {lookAt 으로 위같이 수정 할 수 있다.
    // auto cameraZ = glm::normalize(cameraPos - cameraTarget);
    // auto cameraX = glm::normalize(glm::cross(cameraUp, cameraZ));
    // auto cameraY = glm::cross(cameraZ, cameraX);

    // auto cameraMat = glm::mat4(
    //     glm::vec4(cameraX, 0.0f),
    //     glm::vec4(cameraY, 0.0f),
    //     glm::vec4(cameraZ, 0.0f),
    //     glm::vec4(cameraPos, 1.0f)
    //     );
    // auto view = glm::inverse(cameraMat);
    // }
//camera

    for (size_t i = 0; i < cubePositions.size(); i++) {
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
                glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

bool Context::Init() {

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    };

    uint32_t indices[] = {
        0,  2,  1,  2,  0,  3,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    //vertex array object(vao) 생성 (vertex 버퍼 전에 선언)
    m_vertexLayout = VertexLayout::Create();

    //vertex buffer object(vbo) 생성
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 120);

    //vertex attribute array setting
    //floating pointer 값이 3개, GL_FALSE 노말라이즈 할필요 없음, sizeof(float) * 3 스트라이드: 다음 점에서의 어트르뷰트는 float * 3 개 
    //0번 어트리뷰트를 사용 할꺼고 (simple.vs 의 location = 0)
    //결국 위 어레이 값을 0번째 부터 3개씩 float 값으로 쪼개 simple.vs 에 aPos vec3(vector 3 형태로)에 넣음
    //m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    //postion
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
     //texture coordinate
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);

    //index buffer
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

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
        //minmap : 이미지 크기가 작아지면 나타나는 문제를 해결하는 방법 baselevel 무한등비 급수? 1/3 만큼의 메모리 필요
            // auto image = Image::Create(512, 512);
            // image->SetCheckImage(16, 16);
        //minmap 
    auto image = Image::Load("./images/container.jpg");
    if (!image) {
        return false;
    }

    SPDLOG_INFO("image {}x{}, {} channels",
        image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    //gpu memory 할당
    m_texture = Texture::CreateFromImage(image.get());

    auto image2 = Image::Load("./images/awesomeface.png");
    if (!image2) {
        return false;
    }
    m_texture2 = Texture::CreateFromImage(image2.get());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();
    m_program->SetUniform("tex", 0);
    m_program->SetUniform("tex2", 1);

    // // 위치 (1, 0, 0)의 점. 동차좌표계 사용
    // glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // // 단위행렬 기준 (1, 1, 0)만큼 평행이동하는 행렬
    // auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    // // 단위행렬 기준 z축으로 90도 만큼 회전하는 행렬
    // auto rot = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // // 단위행렬 기준 모든 축에 대해 3배율 확대하는 행렬
    // auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
    // // 확대 -> 회전 -> 평행이동 순으로 점에 선형 변환 적용
    // vec = trans * rot * scale * vec;
    // SPDLOG_INFO("transformed vec: [{}, {}, {}]", vec.x, vec.y, vec.z);

    // 단위 행렬
    // auto transform = glm::mat4(1.0f);
    // 단위행렬 기준 (1, 1, 0)만큼 평행이동하는 행렬
    // auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    // 단위행렬 기준 z축으로 90도 만큼 회전하는 행렬
    // auto transform = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // 단위행렬 기준 모든 축에 대해 3배율 확대하는 행렬
    // auto transform = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));

    // 0.5배 축소후 z축으로 90도 회전하는 행렬
    // auto transform = glm::rotate(
    //     glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)),
    //     glm::radians(90.0f),
    //     glm::vec3(0.0f, 0.0f, 1.0f)
    //     );

    // x 축으로 -55도 회전
    auto model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    // 종회비 4:3 세로화각 45도의 원근 투영
    auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 10.0f); //0.01f near / 10.0f far
    auto transform = projection * view * model;  
    m_program->SetUniform("transform", transform);

    return true;
}

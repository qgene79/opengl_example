#include "common.h"
#include "shader.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h> //glad를 glfw 보다 먼저 인쿨루드
#include <GLFW/glfw3.h>

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}", key , scancode,
        action == GLFW_PRESS ? "Pressed" :  action == GLFW_RELEASE ? "Release" : action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, const char** argv) {
    SPDLOG_INFO("Start program");

    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_INFO("failed to initalize glfw: {}", description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 생성, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME,
      nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    //Opengl context 셋팅
    glfwMakeContextCurrent(window);

    // glad를 활용한 OpenGL 함수 로딩(Opengl context 셋팅 이후 실행)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    // 이 시점 부터 opengl 사용가능
    auto glVersion = glGetString(GL_VERSION);
    //SPDLOG_INFO("OpenGL context version: {}", glVersion); //core.h static_assert failed: 'Formatting of non-void pointers is disallowed.'
    SPDLOG_INFO("OpenGL context version: {}", (const char*)glVersion);

    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    //event callback 함수
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        //전체 Clear 할 색 지정
        glClearColor(0.0f, 0.1f, 0.2f, 0.0f);
        //전체 Clear 실행
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
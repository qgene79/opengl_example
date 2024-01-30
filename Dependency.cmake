# ExternalProject 관련 명령어 셋 추가(ExternalProject_Add)
include(ExternalProject)

# Dependency 관련 변수 설정
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# spdlog: fast logger library
ExternalProject_Add(
# 대표이름     
    dep-spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.x"
# ON(1) OFF(0) commit 의 최신 것을 다운로드     
    GIT_SHALLOW 1 
# 최신코드가 업데이트 되었다면 업데이트 함 (""설정 안함)   
    UPDATE_COMMAND ""
# 최신코드가 패치 되었다면 패치 함 (""설정 안함)       
    PATCH_COMMAND ""
# CMAKE 가 빌드를 다했을 때 인스톨 할 장소 지정    
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
# 자동 테스트 지정 을 위함 (""설정 안함)      
    TEST_COMMAND ""
)
# Dependency 리스트 및 라이브러리 파일 리스트 추가
set(DEP_LIST ${DEP_LIST} dep-spdlog)
# 컨피그 단계가 아닌 빌드 단계에서 실행 되게... config가 debug 면 spdlogd로 표현
set(DEP_LIBS ${DEP_LIBS} spdlog$<$<CONFIG:Debug>:d>)

# glfw
ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.3.2"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        #example은 안받는다
        -DGLFW_BUILD_EXAMPLES=OFF
        #tests는 안받는다
        -DGLFW_BUILD_TESTS=OFF
        #doc은 안받는다
        -DGLFW_BUILD_DOCS=OFF
    )
set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)

# glad
ExternalProject_Add(
    dep_glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad"
    GIT_TAG "v0.1.34"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DGLAD_INSTALL=ON
    TEST_COMMAND ""
    )
set(DEP_LIST ${DEP_LIST} dep_glad)
set(DEP_LIBS ${DEP_LIBS} glad)
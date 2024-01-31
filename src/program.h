#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"

CLASS_PTR(Program);
class Program {
public:
//<ShaderPtr>&에서 & 는 reference type 이다 메모리값만 전달 (포인터와 비슷?)
    static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders);
    
    ~Program();
    uint32_t Get() const { return m_program; }

private:
    Program() {}
    bool Link(const std::vector<ShaderPtr>& shaders);
    uint32_t m_program { 0 };
};

#endif // __PROGRAM_H__
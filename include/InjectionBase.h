#pragma once
#include "utils.h"

class InjectionBase {
public:
    virtual bool Inject(DWORD pid, uintptr_t* baseAddress, std::string dllPath) {
        throw std::runtime_error("Function not implemented");
    }

    virtual bool Inject(DWORD pid, uintptr_t* baseAddress, PVOID dllBuffer) {
        throw std::runtime_error("Function not implemented");
    }
};
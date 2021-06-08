#pragma once
#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <thread>
#include <chrono>

#pragma comment(lib, "Psapi")
#pragma comment(lib, "ntdll")

typedef NTSTATUS(NTAPI* _NtGetNextProcess)(
        _In_ HANDLE ProcessHandle,
        _In_ ACCESS_MASK DesiredAccess,
        _In_ ULONG HandleAttributes,
        _In_ ULONG Flags,
        _Out_ PHANDLE NewProcessHandle
);

template<typename T>
auto GetRoutineAddress(const std::string& routine, const std::string& module) -> T
{
    HMODULE hMod = GetModuleHandleA(module.c_str());
    if (hMod) {
        T pRoutine = (T) GetProcAddress(hMod, routine.c_str());
        if (pRoutine) return pRoutine;
        return nullptr;
    }
    return nullptr;
}

template<typename T = uintptr_t*>
auto GetModuleAddr(const std::string& moduleName, const DWORD& processId) -> T
{
    if (processId == -1) return 0;

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

    if (hSnap) {
        MODULEENTRY32 mod32 {};
        mod32.dwSize = sizeof(mod32);

        if (Module32First(hSnap, &mod32)) {
            do {
                if (strstr(reinterpret_cast<const char*>(mod32.szModule), moduleName.c_str()))
                    return (T)mod32.modBaseAddr;
            } while (Module32Next(hSnap, &mod32));
        }
    }
}

auto GetProcessByName(const std::string& processName) -> DWORD;
auto GetProcessHandle(const DWORD& processId) -> HANDLE;
void ErrorHandling(const char* FunctionName, const char* Message);
auto FileExists(const std::string& filePath) -> bool;
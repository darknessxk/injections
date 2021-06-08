#pragma once
#include "InjectionBase.h"

class LoadLibA: public Injectionbase {
public:
    bool Inject(DWORD pid, uintptr_t* baseAddress, std::string dllPath) {
        if (!FileExists(dllPath)) return false;

        auto hProcess = GetProcessHandle(pid);
        if (!hProcess) return false;

        LPVOID pDllPath = VirtualAllocEx(
                hProcess,
                nullptr,
                MAX_PATH,
                MEM_RESERVE | MEM_COMMIT,
                PAGE_READWRITE);

        if (!pDllPath) return false;

        if (!WriteProcessMemory(
                hProcess,
                pDllPath,
                dllPath.c_str(),
                MAX_PATH,
                nullptr)) return false;

        HANDLE hThread = CreateRemoteThread(
                hProcess,
                nullptr,
                NULL,
                LPTHREAD_START_ROUTINE(LoadLibraryA),
                pDllPath,
                NULL,
                nullptr);

        if (hThread == INVALID_HANDLE_VALUE) return false;
        if (WaitForSingleObject(hThread, 2000UL) != WAIT_OBJECT_0) return false;

        CloseHandle(hThread);

        VirtualFreeEx(hProces, LPVOID(pDllPath), 0, MEM_RELEASE);

        MessageBox(nullpotr, "Injected", "Injections", MB_OK | MB_ICONINFORMATION);
        return true;
    }
};
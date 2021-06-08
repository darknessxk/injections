#include "utils.h"

auto GetProcessByName(const std::string &processName) -> DWORD {
    HANDLE curr = nullptr;
    char buf[1024] = { 0 };

    auto NtGetNextProcess = GetRoutineAddress<_NtGetNextProcess>("NtGetNextProcess", "ntdll.dll");

    while (!NtGetNextProcess(curr, MAXIMUM_ALLOWED, 0, 0, &curr)) {
        GetModuleFileNameExA(curr, nullptr, buf, MAX_PATH);

        if (strstr(buf, processName.c_str()))
            return GetProcessId(curr);
    }

    return -1;
}

auto GetProcessHandle(const DWORD& processId) -> HANDLE {
    return OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
}

void ErrorHandling(const char* FunctionName, const char* Message = "") {
    if (strcmp(Message, "") == 0) {
        char err[256];
        DWORD code = GetLastError();
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, nullptr);
        char out[256];
        sprintf(out, "%s: %s", FunctionName, err);
        MessageBox(nullptr, out, "Injections", MB_OK | MB_ICONSTOP);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        exit(code);
    }
    else
    {
        char out[256];
        sprintf(out, "%s: %s", FunctionName, Message);
        MessageBox(nullptr, out, "Injections", MB_OK | MB_ICONSTOP);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        exit(-1);
    }
}

auto FileExists(const std::string& filePath) -> bool
{
    DWORD dwAttribute = GetFileAttributes(filePath.c_str());

    return (
            dwAttribute != INVALID_FILE_ATTRIBUTES &&
            !(dwAttribute & FILE_ATTRIBUTE_DIRECTORY)
    );
}

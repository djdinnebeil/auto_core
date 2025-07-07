module journey;
import base;
import logger;
import print;
import keyboard;
import <Windows.h>;

/**
 * \brief Creates a new process for the specified executable path.
 *
 * \param path The path to the executable.
 * \return True if the process was created successfully, false otherwise.
 */
bool create_process(const wstring& path) {
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcessW(path.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        print("CreateProcess failed - GetLastError = {}", GetLastError());
        return false;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

/**
 * \brief Starts the journey process.
 *
 * This function starts the journey process by creating a new process for the specified executable.
 * It waits for the child process to terminate before returning.
 */
void start_journey() {
    STARTUPINFO si_journey;
    PROCESS_INFORMATION pi_journey;
    ZeroMemory(&si_journey, sizeof(si_journey));
    si_journey.cb = sizeof(si_journey);
    ZeroMemory(&pi_journey, sizeof(pi_journey));
    TCHAR journey_app_name[] = TEXT(R"(C:\DJ\My Folder\Auto Core\bin\auto_core.exe child)");
    if (!CreateProcess(nullptr, journey_app_name, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si_journey, &pi_journey)) {
        print("CreateProcess for journey.exe failed ({})", GetLastError());
        return;
    }
    HANDLE auto_core_journey_process = pi_journey.hProcess;

    // Wait for the child process to terminate
    WaitForSingleObject(auto_core_journey_process, INFINITE);

    CloseHandle(pi_journey.hProcess);
    CloseHandle(pi_journey.hThread);
}

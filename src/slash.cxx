module slash;
import base;
import config;
import logger;
import print;
import clipboard;
import utils;
import <Windows.h>;

/**
 * \brief Calls the external slash executable.
 *
 * This function starts the external slash executable, waits for it to complete, and handles errors if the process creation fails.
 *
 * \param exe_path The path to the slash executable.
 */
void call_slash_exe(const wstring& exe_path) {
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    LPWSTR szCmdline = _wcsdup(exe_path.c_str());

    // Create the slash process
    if (!CreateProcessW(NULL, szCmdline, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        print("error with slash module - {}", GetLastError());
    }
    else {
        // Wait for the process to complete
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    free(szCmdline);
}


/**
 * \brief Retrieves and deletes the contents of the Recycle Bin.
 *
 * This function logs the action, then calls the external slash executable to empty the Recycle Bin.
 *
 * \runtime
 */
void retrieve_and_delete_recycle_bin() {
    logg("retrieve_and_delete_recycle_bin()");
    call_slash_exe(LR"(.\bin\slash.exe)");
}

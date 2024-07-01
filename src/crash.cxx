module crash;
import visual;
import main;
import logger_x;
import <Windows.h>;
import <filesystem>;
import <fstream>;
import <iostream>;
import <format>;

std::string crash_log = R"(.\log\crash.log)";
std::string crash_folder = R"(.\log\crash\)";

/**
 * \brief Checks if a crash log exists and prompts the user if it does.
 *
 * This function pauses the operation of the program if a crash log is detected.
 * It ensures that Auto Core does not get caught in an infinite loop by prompting
 * the user to acknowledge the crash before continuing. This behavior is crucial
 * for preventing the program from repeatedly restarting without intervention.
 */
void crash_check() {
    if (std::filesystem::exists(crash_log)) {
        print("Crash detected --- press any key to continue");
        std::cin.get(); // Pause and wait for user input
    }
}

/**
 * \brief Restarts the program after a crash.
 *
 * Logs the crash information, copies necessary files, and restarts the program.
 *
 * \param error_report The error report to log.
 */
void restart_program(const std::string& error_report) {
    // Log the crash information
    std::ofstream log_crash(crash_log, std::ios_base::app);
    if (log_crash.is_open()) {
        std::string crash_report = std::format("{}\ncrash at {} {}", error_report, get_timestamp_with_seconds(), get_datestamp());
        logg(crash_report);
        log_crash << crash_report;
        log_crash.close();
    }
    // Restart the program
    std::wstring restart_cmd = L"auto_core.exe";
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (CreateProcessW(NULL, restart_cmd.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        close_program();
        logg("restarting auto_core.exe at {}", get_datetime_stamp_with_seconds());
        log_end();
        ExitProcess(1);
    }
}

/**
 * \brief Generates a unique crash dump file name.
 *
 * Ensures that the crash dump file name is unique to avoid overwriting existing files.
 *
 * \return The generated crash dump file name.
 */
std::string get_crash_name() {
    std::string crash_name = "bandicoot_" + get_datestamp();
    std::string crash_path = crash_folder + crash_name + ".dmp";
    int counter = 0;
    while (std::filesystem::exists(crash_path)) {
        counter++;
        crash_name = "bandicoot_" + get_datestamp() + "_" + std::to_string(counter);
        crash_path = crash_folder + crash_name + ".dmp";
    }
    return crash_name;
}

/**
 * \brief Unhandled exception handler.
 *
 * Handles unhandled exceptions, generates a crash dump, and restarts the program.
 *
 * \param exceptionInfo Pointer to the exception information.
 * \return The exception execution handler.
 */
LONG WINAPI unhandled_exception_handler(EXCEPTION_POINTERS* exceptionInfo) {
    // Create the crash folder if it does not exist
    std::wstring crash_folder_wstr = std::wstring(crash_folder.begin(), crash_folder.end());
    CreateDirectoryW(crash_folder_wstr.c_str(), NULL);
    // Generate a unique crash dump file name
    std::string crash_dmt_name = get_crash_name();
    std::wstring crash_log_name = std::wstring(crash_dmt_name.begin(), crash_dmt_name.end());
    std::wstring filepath = crash_folder_wstr + crash_log_name + L".dmp";
    std::wstring crash_folder_crash_files = crash_folder_wstr + crash_log_name + L"\\";
    CreateDirectoryW(crash_folder_crash_files.c_str(), NULL);
    // Copy the executable and PDB files
    std::wstring current_exe_path = LR"(.\bin\auto_core.exe)";
    std::wstring copy_exe_path = crash_folder_crash_files + L"auto_core.exe";
    std::wstring current_pdb_path = LR"(.\visual\auto_core.pdb)";
    std::wstring copy_pdb_path = crash_folder_crash_files + L"auto_core.pdb";
    CopyFileW(current_exe_path.c_str(), copy_exe_path.c_str(), FALSE);
    CopyFileW(current_pdb_path.c_str(), copy_pdb_path.c_str(), FALSE);
    // Log the error report and restart the program
    std::string error_report = "Unhandled exception occurred.";
    error_report += std::format("\nSee {} for a crash log", crash_dmt_name);
    error_report += "\nException Code: " + std::to_string(exceptionInfo->ExceptionRecord->ExceptionCode);
    error_report += "\nException Address: " + std::to_string(reinterpret_cast<uintptr_t>(exceptionInfo->ExceptionRecord->ExceptionAddress));
    restart_program(error_report);
    return EXCEPTION_EXECUTE_HANDLER;
}

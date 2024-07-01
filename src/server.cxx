module server;
import base;
import journey;
import visual;
import <Windows.h>;
import <string>;
import <fstream>;
import <iostream>;
import <format>;
namespace fs = std::filesystem;

PROCESS_INFORMATION pi_server; // Process information for the server process

void log_server_msg(const std::string& msg, bool end_server_process = false, bool send_to_print = false) {
    std::ofstream server_log_stream;
    std::string server_log_name = "server_" + get_datestamp() + ".log";
    std::string server_log_directory = R"(.\log\server\)";
    std::string server_log_path = server_log_directory + server_log_name;
    server_log_stream.open(server_log_path, std::ios::app);
    server_log_stream << msg << std::endl;
    if (end_server_process) {
        server_log_stream << "Session ended at " << get_datetime_stamp_for_logger() << "\n" << "***" << std::endl;
    }
    server_log_stream.flush();
    server_log_stream.close();
    logg(msg);
    if (send_to_print) {
        print(msg);
    }
}

void start_server() {
    std::wstring server_path = LR"(.\bin\server.exe)";

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessW(server_path.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        log_server_msg("server start failed", false, true);
    }
    else {
        pi_server = pi; // Store the process information globally
        log_server_msg("server started successfully", false);
    }
}

void stop_server() {
    if (pi_server.hProcess) {
        if (TerminateProcess(pi_server.hProcess, 0)) {
            log_server_msg("server process terminated", true);
        }
        else {
            DWORD error = GetLastError();
            log_server_msg(std::format("Failed to terminate server process - GetLastError() = {}", error));
        }
        CloseHandle(pi_server.hProcess);
        CloseHandle(pi_server.hThread);
    }
    else {
        log_server_msg("No server process to terminate");
    }
}

export module pipes_x;
import base;
import config;
import logger;
import print;
import utils;
import <Windows.h>;

export unordered_map<int, function<void()>> command_map;
export bool end_process;

export {
    HANDLE connect_to_pipe_server(const wstring& pipe_name);
    void process_pipe_commands(HANDLE h_pipe);
}

/**
 * \brief Connects to a named pipe server.
 *
 * This function connects to a named pipe server with the specified pipe name.
 *
 * \param pipe_name The name of the pipe.
 * \return The handle to the connected pipe server, or NULL if connection failed.
 */
HANDLE connect_to_pipe_server(const wstring& pipe_name) {
    HANDLE h_pipe = CreateFile(
        (LR"(\\.\pipe\)" + pipe_name).c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);
    if (h_pipe == INVALID_HANDLE_VALUE) {
        print("Failed to connect to pipe '{}'. Error: {}", pipe_name, GetLastError());
        return NULL;
    }
    return h_pipe;
}

/**
 * \brief Processes commands received through a pipe.
 *
 * This function reads commands from the specified pipe and executes the corresponding actions.
 * It continues to read and process commands until the end_process flag is set.
 *
 * \param h_pipe The handle to the pipe.
 */
void process_pipe_commands(HANDLE h_pipe) {
    int command;
    DWORD bytesRead;
    while (true) {
        BOOL success = ReadFile(h_pipe, &command, sizeof(command), &bytesRead, NULL);
        if (success && bytesRead == sizeof(command)) {
            auto action = command_map.find(command);
            if (action != command_map.end()) {
                action->second();
            }
            else {
                print("Invalid command received.");
            }
        }
        else {
            print("Failed to read command. Error: {}", GetLastError());
            break;
        }
        if (end_process) {
            break;
        }
    }
}

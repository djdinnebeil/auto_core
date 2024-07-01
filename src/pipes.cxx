module pipes;
import base;
import config;
import logger;
import print;
import utils;
import <Windows.h>;

/**
 * \brief Creates a named pipe server.
 *
 * This function creates a named pipe server with the specified pipe name.
 *
 * \param pipe_name The name of the pipe.
 * \return The handle to the created pipe server, or NULL if creation failed.
 */
HANDLE create_pipe_server(const wstring& pipe_name) {
    HANDLE h_pipe = CreateNamedPipeW(
        (LR"(\\.\pipe\)" + pipe_name).c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1, 1024, 1024, 0, NULL);

    if (h_pipe == INVALID_HANDLE_VALUE) {
        print("Failed to create pipe '{}'. Error: {}", pipe_name, GetLastError());
        return NULL;
    }
    logg("pipe '{}' created", pipe_name);
    return h_pipe;
}

/**
 * \brief Sends a command through a pipe.
 *
 * This function sends a command to the specified pipe.
 *
 * \param h_pipe The handle to the pipe.
 * \param command The command to send.
 * \return true if the command was successfully sent, false otherwise.
 */
bool send_pipe_command(HANDLE h_pipe, int command) {
    DWORD bytesWritten;
    return WriteFile(h_pipe, &command, sizeof(command), &bytesWritten, NULL) && (bytesWritten == sizeof(command));
}

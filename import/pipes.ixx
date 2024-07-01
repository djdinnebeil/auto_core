/**
 * \file pipes.ixx
 * \brief This module provides support for inter-process communication using system pipes.
 *
 * This module enables inter-process communication (IPC) through named pipes in Windows.
 * It provides functions for creating and connecting to pipe servers, sending commands, and processing commands.
 */
export module pipes;
import base;
import config;
import logger;
import print;
import utils;
import <Windows.h>;

export {
    HANDLE create_pipe_server(const wstring& pipe_name);
    bool send_pipe_command(HANDLE h_pipe, int command);
}

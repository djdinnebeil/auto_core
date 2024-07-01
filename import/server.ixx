/**
 * \file server.ixx
 * \brief Sets up a local server facilitating web-based local file access.
 *
 * This module provides functions to start and stop a local server process,
 * facilitating web-based access to local files. It also logs server-related events.
 */
export module server;
import base;
import journey;
import visual;
import <Windows.h>;

export {
    void start_server();
    void stop_server();
}

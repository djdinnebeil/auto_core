/**
\file server.cxx
\brief This file contains the implementation of the server component using CivetServer.
 
The server component initializes and runs a local server based on the configuration
specified in the server.ini file. This enables local file access through a web browser.

\warning Ensure that the configuration file server.ini is properly formatted
and located in the config directory.
 */
 /**
 \file server.cxx
 \brief This file contains the implementation of the server component using CivetServer.

 The server component initializes and runs a local server based on the configuration
 specified in the server.ini file. This enables local file access through a web browser.

 \warning Ensure that the configuration file server.ini is properly formatted
 and located in the config directory.
 */
import base;
import config;
import clock;
import logger;
import clipboard;
import print;
import utils;
import keyboard;
import server_x;
#pragma warning(disable:4251)
#pragma warning(disable:4275)
import <CivetServer.h>;

CivetServer* server;

/**
 * \brief Runs the server.
 * Initializes the server with the configuration options and starts the server loop.
 */
void run_server() {
    string port_number_str = to_string(config.port_number);
    const char* options[] = {
        "document_root", R"(.\server\)",
        "listening_ports", port_number_str.c_str(),
        NULL
    };
    string current_datestamp = get_datestamp();
    try {
        server = new CivetServer(options);
        server_logger.logg_and_logg("server started on port {}", options[3]);
        while (true) {
            this_thread::sleep_for(chrono::seconds(60));
            string datestamp = get_datestamp();
            if (datestamp != current_datestamp) {
                server_logger.update_log_file();
                current_datestamp = datestamp;
            }
        }
    }
    catch (exception& e) {
        print("Exception caught in server: {}", e.what());
        exit(1);
    }
}

int main() {
    log_init();
    run_server();
    return 0;
}
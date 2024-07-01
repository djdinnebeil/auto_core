/**
 * \file logger_t.ixx
 * \brief This module creates a logger thread, updates component logs, and initializes logging.
 *
 * This module defines functions that create and manage a logging thread. The thread
 * logs activities at regular intervals, updates log files, and manages log rotation.
 * It also initializes the logging system.
 */
export module logger_t;
import base;
import config;
import clock;
import logger;
import logger_c;
import logger_x;
import <Windows.h>;

export {
    void log_init();
    void log_start();
}

/**
 * \file logger_x.ixx
 * \brief This module updates component logs.
 *
 * This module provides functionality to update individual logging components, such as iTunes and Spotify loggers.
 * It ensures that the logs for these components are kept up-to-date and properly managed.
 */
export module logger_x;
import base;
import config;
import clock;
import logger;
import logger_c;
import sp;
import itunes;
export import wake;

export {
    void update_log_components();
}

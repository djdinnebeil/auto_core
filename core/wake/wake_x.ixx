/**
\file wake_x.ixx
\brief Provides logging support and initialization for the Spotify component.

This module defines a custom logger for the Spotify component and provides functions
to update the logger and initialize logging when the component starts.
 */
export module wake_x;
import base;
import config;
import clock;
import logger;
import logger_c;
import print;

/**
 * \brief Custom logger for the Spotify component.
 *
 * This logger is used to log messages specific to the Spotify component.
 */
export Logger wake_logger("wake");

/**
 * \brief Updates the Spotify logger.
 *
 * This function updates the main log file and the Spotify logger file.
 */
export void update_wake_logger() {
	update_main_log_file();
	wake_logger.update_log_file();
}

/**
 * \brief Initializes logging for the Spotify component.
 *
 * This function updates the main log file and logs the start of the Spotify component.
 */
export void log_init() {
	update_main_log_file();
	wake_logger.logg_and_logg("wake.exe started");
}

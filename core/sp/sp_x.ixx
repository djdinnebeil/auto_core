/**
\file sp_x.ixx
\brief Provides logging support and initialization for the Spotify component.

This module defines a custom logger for the Spotify component and provides functions
to update the logger and initialize logging when the component starts.
 */
export module sp_x;
import logger;
import logger_c;

/**
 * \brief Custom logger for the Spotify component.
 *
 * This logger is used to log messages specific to the Spotify component.
 */
export Logger sp_logger("sp");

/**
 * \brief Updates the Spotify logger.
 *
 * This function updates the main log file and the Spotify logger file.
 */
export void update_sp_logger() {
	update_main_log_file();
	sp_logger.update_log_file();
}

/**
 * \brief Initializes logging for the Spotify component.
 *
 * This function updates the main log file and logs the start of the Spotify component.
 */
export void log_init() {
	update_main_log_file();
	sp_logger.logg_and_logg("sp_ac.exe started");
}

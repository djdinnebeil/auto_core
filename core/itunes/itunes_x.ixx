/**
\file itunes_x.ixx
\brief Provides logging support and initialization for the iTunes component.
 
This module defines a custom logger for the iTunes component and provides functions
to update the logger and initialize logging when the component starts.
 */
export module itunes_x;
import logger;
import logger_c;

/**
 * \brief Custom logger for the iTunes component.
 *
 * This logger is used to log messages specific to the iTunes component.
 */
export Logger iTunes_logger("itunes");

/**
 * \brief Updates the iTunes logger.
 *
 * This function updates the main log file and the iTunes logger file.
 */
export void update_iTunes_logger() {
	update_main_log_file();
	iTunes_logger.update_log_file();
}
/**
 * \brief Initializes logging for the iTunes component.
 *
 * This function updates the main log file and logs the start of the iTunes component.
 */
export void log_init() {
	update_main_log_file();
	iTunes_logger.logg_and_logg("ac_itunes.exe started");
}

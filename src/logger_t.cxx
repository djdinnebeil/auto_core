module logger_t;
import base;
import config;
import clock;
import logger;
import logger_c;
import logger_x;
import <Windows.h>;

/**
 * \brief Logging thread function.
 *
 * This function runs in a separate thread and logs activities at regular intervals.
 * It updates log files and manages log rotation based on the current date.
 */
void log_thread() {
    Logger logger_logger("logger");
    int log_thread_sleep_time = 60;
    int log_thread_after_60_minutes = 60;
    int log_minutes_count = 0;
    string current_datestamp = get_datestamp();
    logger_logger.logg_and_logg("logger thread started");
    while (true) {
        int seconds_to_sleep = log_thread_sleep_time - get_current_seconds();
        send_logg_wake_signal();
        log_minutes_count++;
        if (log_minutes_count == log_thread_after_60_minutes) {
            logger_logger.logg_and_logg("{} minutes have passed - log thread sleeping for {} seconds at {}", log_thread_after_60_minutes, seconds_to_sleep, get_timestamp_with_seconds());
            log_minutes_count = 0;
        }
        else {
            logger_logger.logg("log thread sleeping for {} seconds at {}", seconds_to_sleep, get_timestamp_with_seconds());
        }
        this_thread::sleep_for(chrono::seconds(seconds_to_sleep));
        current_datestamp = get_datestamp();
        if (current_datestamp != logger_datestamp) {
            logg("---");
            update_main_log_file();
            logger_logger.update_log_file();
            update_log_components();
            logg("---");
            logg(session_start);
        }
    }
}

/**
 * \brief Initializes the logging system.
 *
 * This function sets up the logging environment by setting the session start time, creating necessary
 * directories, updating the main log file, and starting the logging thread.
 */
void log_init() {
    session_start = "Session started " + get_datetime_stamp_for_logger();
    fs::create_directories(log_directory);
    update_main_log_file();
    if (config.send_logg_to_cout) {
        logg("***send logg to output enabled***");
    }
    loggnl(config.configuration_log);
    thread t(log_thread);
    t.detach();
}

/**
 * \brief Starts the logging session.
 *
 * This function initializes the session start time, creates necessary directories,
 * updates the main log file, and logs the session start message.
 */
void log_start() {
    session_start = "Session started " + get_datetime_stamp_for_logger();
    fs::create_directories(log_directory);
    update_main_log_file();
    logg(session_start);
}

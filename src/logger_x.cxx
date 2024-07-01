module logger_x;
import base;
import config;
import clock;
import logger;
import logger_c;
import sp;
import itunes;
import wake;
import <Windows.h>;

/**
 * \brief Updates individual logging components.
 *
 * This function updates the loggers for individual components, such as Spotify and iTunes,
 * ensuring their logs are up-to-date and properly managed.
 */
void update_log_components() {
    update_sp_logger();
    update_iTunes_logger();
    update_wake_logger();
}

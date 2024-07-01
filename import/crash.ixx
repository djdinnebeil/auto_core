/**
 * \file crash.ixx
 * \brief Provides crash handling support for Auto Core. By default, Auto Core is set to restart if there is a crash due to the role as a keyboard manager.
 *
 * This module includes functions to handle unhandled exceptions, restart the program, and check for crash logs.
 */
export module crash;
import visual;
import main;
import logger_x;
import <Windows.h>;

export {
    LONG WINAPI unhandled_exception_handler(EXCEPTION_POINTERS* exceptionInfo);
    void restart_program(const string& error_report);
    void crash_check();
}

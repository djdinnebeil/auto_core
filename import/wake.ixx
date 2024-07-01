/**
 * \file wake.ixx
 * \brief Module for managing the wake component in the Auto Core application.
 *
 * This module provides functions to start, control, and interact with the wake component,
 * which is responsible for handling specific system wake events and logging.
 * The wake component is executed as a separate process and communicates with the main application
 * through named pipes.
 */
export module wake;
import visual;
import pipes;
import journey;
import <Windows.h>;

export HANDLE wake_pipe;

export {
    void start_wake_component();
    void send_wake_end_signal();
    void send_logg_wake_signal();
    void update_wake_logger();
}

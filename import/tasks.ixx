/**
 * \file link.ixx
 * \brief Generates custom user-defined strings for use within the Auto Core application.
 *
 * This module provides functions to generate and print messages for journaling inspiration
 * and task lists. It interacts with the clipboard and logs the generated messages.
 */
export module tasks;
import base;
import config;
import clipboard;
import logger;
import print;
import thread;
import main;
import <Windows.h>;

export {
    void launch_task_list();
    void print_task_list();
    string get_task_list();
}

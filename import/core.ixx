/**
 * \file core.ixx
 * \brief This module centralizes main program imports. 
*/
export module core;

export import base;
export import config;
export import logger;
export import visual;
export import main;
export import journal;
export import link;
export import tasks;
export import slash;
export import numkey;
export import taskbar;
export import itunes;
export import sp;
export import star;
export import taskbar;
export import taskbar_ps;
export import taskbar_11;
export import wake;
export import notes;

export import pipes;
export import cloud;
export import print;
export import utils;
export import server;
export import crash;
export import keyboard;
export import thread;

export import logger_c;
export import logger_x;
export import logger_t;

using std::string_view;

/**
 * \brief Placeholder structure to provide symmetry in Visual Studio. This can safely be removed.
 */
struct ide_placeholder {};
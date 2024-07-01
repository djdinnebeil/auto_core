/**
\file visual.ixx
\brief This module groups essential utility modules.
This module centralizes the import of several essential utility modules, making it
easier to manage and import them collectively. By importing 'visual', all the 
grouped modules are included, simplifying the import process for other parts of the application.
\hardlink
*/
export module visual;
export import base;
export import config;
export import clock;
export import logger;
export import clipboard;
export import print;
export import utils;
export import keyboard;

/**
 * \brief Placeholder structure to provide symmetry in Visual Studio.
 *
 * The ide_placeholder struct is included to provide a visual cue in Visual Studio's
 * Solution Explorer, ensuring that the module has an expandable arrow next to it.
 */
struct ide_placeholder {};
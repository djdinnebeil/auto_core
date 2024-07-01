/**
 * \file dash.ixx
 * \brief This module is used to initialize runtime configuration, if enabled.
 *
 * This module reads configuration settings from the runtime_map.ini file and sets
 * action mappings based on the configuration. It is only used if runtime configuration
 * is enabled.
 */
export module dash;
import core;
import auto_core;
import dash_x;
import <Windows.h>;

export {
    void parse_and_set_action_map();
}

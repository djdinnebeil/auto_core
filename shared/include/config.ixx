/**
 * \file config.ixx
 * \brief Provides a module to handle program configuration.
 *
 * This module defines a Config class to manage global configuration settings
 * for the application. It includes methods to load configuration settings from
 * various configuration files.
 * \hardlink
 */
module;

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

export module config;
import base;
import <Windows.h>;

const wstring current_directory = LR"(C:\DJ\My Folder\Auto Core)"; 
const wstring program_title = L"Auto Core";

/**
 * \brief Config class to manage global configuration settings.
 */
export class Config {
public:
    static DLL_API Config& get_instance();
    string configuration_log;
    wstring current_directory;
    wstring program_title;
    bool runtime_enabled;
    bool start_server;
    int port_number;
    int end_of_day;
    bool send_logg_to_cout;
    unordered_map<string, int> taskbar_position;
    unordered_map<int, string> taskbar_program;
private:
    Config();
    void load_runtime_config();
    void load_server_config();
    void load_clock_config();
    void load_logger_config();
    void load_taskbar_config();
};

export DLL_API Config& config = Config::get_instance();

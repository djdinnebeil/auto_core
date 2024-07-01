module config;
import base;
import <Windows.h>;

/**
 * \brief Retrieves the singleton instance of the Config class.
 * \return The singleton instance of the Config class.
 */
Config& Config::get_instance() {
    static Config instance;
    return instance;
}

/**
 * \brief Constructs the Config object and initializes configuration settings.
 *
 * The constructor sets default values for the configuration settings and
 * loads configuration settings from the respective configuration files.
 */
Config::Config()
    : current_directory(::current_directory),
    program_title(::program_title),
    runtime_enabled(false),
    start_server(false),
    configuration_log("Configuration log:\n")
{
    SetCurrentDirectoryW(current_directory.c_str());
    SetConsoleTitleW(program_title.c_str());
    load_runtime_config();
    load_server_config();
    load_clock_config();
    load_logger_config();
    load_taskbar_config();
}

/**
 * \brief Loads runtime configuration settings from the runtime.ini file.
 */
void Config::load_runtime_config() {
    ifstream runtime_file(R"(.\config\runtime.ini)");
    if (!runtime_file) {
        return;
    }
    string line;
    getline(runtime_file, line);
    auto open_bracket = line.find('[');
    auto close_bracket = line.find(']');
    string value = line.substr(open_bracket + 1, close_bracket - open_bracket - 1);
    runtime_enabled = value == "true";
    runtime_file.close();
    configuration_log += "runtime values set\n";
}

/**
 * \brief Loads server configuration settings from the server.ini file.
 */
void Config::load_server_config() {
    ifstream server_file(R"(.\config\server.ini)");
    string line;
    getline(server_file, line);
    auto open_bracket = line.find('[');
    auto close_bracket = line.find(']');
    string value = line.substr(open_bracket + 1, close_bracket - open_bracket - 1);
    start_server = (value == "true");
    getline(server_file, line);
    open_bracket = line.find('[');
    close_bracket = line.find(']');
    value = line.substr(open_bracket + 1, close_bracket - open_bracket - 1);
    port_number = stoi(value);
    server_file.close();
    configuration_log += "server values set\n";
}

/**
 * \brief Loads clock configuration settings from the clock.ini file.
 */
void Config::load_clock_config() {
    ifstream clock_file(R"(.\config\clock.ini)");
    string line;
    getline(clock_file, line);
    auto open_bracket = line.find('[');
    auto close_bracket = line.find(']');
    string value = line.substr(open_bracket + 1, close_bracket - open_bracket - 1);
    end_of_day = stoi(value);
    clock_file.close();
    configuration_log += "clock values set\n";
}

/**
 * \brief Loads logger configuration settings from the logger.ini file.
 */
void Config::load_logger_config() {
    ifstream logger_file(R"(.\config\logger.ini)");
    string line;
    getline(logger_file, line);
    auto open_bracket = line.find('[');
    auto close_bracket = line.find(']');
    string value = line.substr(open_bracket + 1, close_bracket - open_bracket - 1);
    send_logg_to_cout = value == "true";
    logger_file.close();
    configuration_log += "logger values set\n";
}

/**
 * \brief Sets the taskbar positions based on the configuration file.
 */
void Config::load_taskbar_config() {
    ifstream taskbar_config_file(R"(.\config\taskbar.ini)");
    string line;
    while (getline(taskbar_config_file, line)) {
        size_t first_space = line.find(' ');
        size_t second_space = line.rfind(' ');
        int number = stoi(line.substr(0, first_space));
        if (number == 10) {
            number = 0;
        }
        string name = line.substr(second_space + 1);
        taskbar_position[name] = number;
        taskbar_program[number] = name;
    }
    taskbar_config_file.close();
    configuration_log += "taskbar values set\n";
}

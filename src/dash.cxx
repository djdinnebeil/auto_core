module dash;
import core;
import auto_core;
import dash_x;
import config;
import <Windows.h>;

// Internal helper to parse a runtime_map.ini line
static bool parse_line(const string& line, string& key_string, string& primary, string& secondary) {
    size_t opening_bracket = line.find('[');
    size_t closing_bracket = line.find(']');
    if (opening_bracket == string::npos || closing_bracket == string::npos || closing_bracket <= opening_bracket) {
        return false;
    }
    key_string = line.substr(opening_bracket + 1, closing_bracket - opening_bracket - 1);
    size_t opening_brace = line.find('{');
    size_t closing_brace = line.find('}');
    if (opening_brace == string::npos || closing_brace == string::npos || closing_brace <= opening_brace) {
        return false;
    }
    size_t comma_after_parenthese = line.find("),");
    size_t comma_delimiter = line.find(',');
    if (comma_after_parenthese != string::npos) {
        comma_delimiter = comma_after_parenthese + 1;
    }
    if (comma_delimiter == string::npos || comma_delimiter <= opening_brace || comma_delimiter >= closing_brace) {
        return false;
    }
    primary = line.substr(opening_brace + 1, comma_delimiter - opening_brace - 1);
    secondary = line.substr(comma_delimiter + 2, closing_brace - comma_delimiter - 2);
    return true;
}

/**
 * \brief Parses the runtime configuration file and sets the action map.
 *
 * This function reads the runtime_map.ini file, parses the configuration settings,
 * and sets the action map for the numpad keys based on the parsed configuration.
 */
void parse_and_set_action_map() {
    ifstream config_file(R"(.\config\runtime_map.ini)");
    string line;
    oss log_buffer;
    bool log_buffer_empty = true;
    while (getline(config_file, line)) {
        string key_string, primary, secondary;
        if (!parse_line(line, key_string, primary, secondary)) {
            logg("Invalid line format: {}", line);
            continue;
        }
        int key = get_numkey_vk_code(key_string);
        if (key != -1) {
            ac_numkey_event[key] = {get_function_by_name(primary), get_function_by_name(secondary)};
        }

        if (config.runtime_debugger) {
            logg("{} = {}, {}", key, primary, secondary);
        }
        else if (config.runtime_logger) {
            if (!log_buffer_empty) {
                log_buffer << "\n";
            }
            log_buffer << key << " = " << primary << ", " << secondary;
            log_buffer_empty = false;
        }
    }
    if (config.runtime_debugger) {
        print("runtime logger set to debug mode");
    }
    else if (config.runtime_logger) {
        logg(log_buffer.str());
    }
    logg("runtime map configured");
    config_file.close();
}

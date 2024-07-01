/**
\file wake.cxx
\brief Logs system wake events, extending Windows' native logging to offer historical wake event tracking.

By polling system information, this module provides detailed logging of wake events,
aiding in the analysis of system behavior and Auto Core's interaction with the host machine.
*/
import visual;
import pipes_x;
import wake_x;
import <Windows.h>;

wstring pipe_name = L"wake_pipe";

/**
 * \brief Ends the wake component.
 */
void end_wake() {
    wake_logger.logg("wake_logger is shutting down");
    end_process = true;
}

/**
 * \brief Logs the last wake event.
 *
 * This function retrieves the last wake event information from the system and logs it.
 * If a change in wake state is detected, it updates the log files and logs the new wake event.
 */
void log_last_wake() {
    wake_logger.logg("Checking last wake log at {}", get_timestamp_with_seconds());
    string wake_directory = R"(C:\DJ\My Folder\Auto Core\log\wake\)";
    fs::create_directories(wake_directory);
    static string previous_last_wake_file = wake_directory + "previous_wake.log";
    static string current_last_wake_file = wake_directory + "current_wake.log";
    static string last_wake_log_file = wake_directory + "wake_master_log.log";
    ofstream current_last_wake_clear(current_last_wake_file);
    if (current_last_wake_clear.is_open()) {
        current_last_wake_clear << "";
        current_last_wake_clear.flush();
        current_last_wake_clear.close();
    }
    static string retrieve_last_wake_command = format("powercfg /lastwake >> \"{}\"", current_last_wake_file);
    system(retrieve_last_wake_command.c_str());
    ifstream previous_last_wake_stream(previous_last_wake_file);
    string line;
    oss previous_last_wake_oss;
    while (getline(previous_last_wake_stream, line)) {
        previous_last_wake_oss << line << '\n';
    }
    previous_last_wake_stream.close();
    ifstream current_last_wake_stream(current_last_wake_file);
    oss current_last_wake_oss;
    while (getline(current_last_wake_stream, line)) {
        current_last_wake_oss << line << '\n';
    }
    current_last_wake_stream.close();
    string current_last_wake_str = current_last_wake_oss.str();
    string current_last_wake_output;
    if (current_last_wake_str != previous_last_wake_oss.str()) {
        current_last_wake_output = get_datetime_stamp_with_seconds() + '\n' + current_last_wake_str;
        ofstream last_wake_log_stream(last_wake_log_file, ios::app);
        if (last_wake_log_stream.is_open()) {
            last_wake_log_stream << current_last_wake_output;
            last_wake_log_stream.flush();
            last_wake_log_stream.close();
        }
        ofstream previous_last_wake_update(previous_last_wake_file);
        if (previous_last_wake_update.is_open()) {
            previous_last_wake_update << current_last_wake_str;
            previous_last_wake_update.flush();
            previous_last_wake_update.close();
        }
        wake_logger.loggnl_and_loggnl("wake state change detected at {}", current_last_wake_output);
    }
    wake_logger.log_stream.flush();
    main_log_stream.flush();
}

/**
 * \brief Sets up the command map for handling pipe commands.
 *
 * This function maps integer command IDs to corresponding functions that handle
 * specific commands for the iTunes component.
 */
void set_command_map() {
    command_map[0] = {[]() {  end_wake(); }};
    command_map[1] = {log_last_wake};
    command_map[2] = {update_wake_logger};
}

int main() {
    log_init();
    log_last_wake();
    set_command_map();
    HANDLE wake_pipe = connect_to_pipe_server(pipe_name);
    if (wake_pipe != NULL) {
        wake_logger.logg_and_logg("connected to pipe '{}' server", pipe_name);
        process_pipe_commands(wake_pipe);
    }
    else {
        wake_logger.logg_and_print("Failed to connect to pipe server.");
    }
    wake_logger.logg_and_logg("wake.exe has ended");
    wake_logger.close_log_file();
    CloseHandle(wake_pipe);
    close_main_log_file();
    return 0;
}

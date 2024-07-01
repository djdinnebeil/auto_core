/**
\file itunes.cxx
\brief Entry point for the iTunes component of Auto Core.

This file initializes the iTunes component, sets up the command map for handling
various commands through named pipes, and manages the main loop for processing
pipe commands. It also handles logging initialization and shutdown.
 */
import visual;
import itunes_c;
import itunes_t;
import slash_i;
import itunes_x;
import pipes_x;
import <Windows.h>;

/** Name of the pipe for communication with the main program */
wstring pipe_name = L"ac_itunes_pipe";

/**
 * \brief Shuts down the iTunes component.
 *
 * This function logs the shutdown message and sets the end_process flag to true.
 */
void end_iTunes() {
    iTunes_logger.logg("iTunes is shutting down");
    end_process = true;
}

/**
 * \brief Sets up the command map for handling pipe commands.
 *
 * This function maps integer command IDs to corresponding functions that handle
 * specific commands for the iTunes component.
 */
void set_command_map() {
    command_map[0] = {[]() {end_iTunes();}};
    command_map[1] = {iTunes_play_pause};
    command_map[2] = {iTunes_next_song};
    command_map[3] = {print_iTunes_songs};
    command_map[4] = {print_next_up_song_list};
    command_map[5] = {update_iTunes_logger};
    command_map[6] = {iTunes_prev_song};
    command_map[9] = {remove_iTunes_song};
}

/**
 * \brief Entry point for the iTunes component.
 *
 * This function initializes logging, sets up the command map, connects to the named pipe server,
 * processes pipe commands, and handles shutdown and cleanup.
 *1
 * \return int Exit status code.
 */
int main() {
    log_init();
    set_command_map();
    HANDLE ac_itunes_pipe = connect_to_pipe_server(pipe_name); 
    if (ac_itunes_pipe != NULL) {
        iTunes_logger.logg_and_logg("connected to pipe '{}' server", pipe_name);
        process_pipe_commands(ac_itunes_pipe);
    }
    else {
        print("Failed to connect to pipe server.");
    }
    ac_iTunes.finalize_com();
    iTunes_logger.logg_and_logg("ac_itunes.exe has ended");
    iTunes_logger.close_log_file();
    CloseHandle(ac_itunes_pipe);
    close_main_log_file();
    return 0;
}


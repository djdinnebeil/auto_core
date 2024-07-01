/**
\file auto_core.ixx
\brief The main module.
*/
export module auto_core;
export import core;

/**
 * \brief Struct to hold primary and secondary function states.
 */
export struct function_state {
    function<void()> primary;
    function<void()> secondary;
};

/**
 * \brief Unordered map to store function states for numkey events.
 */
export unordered_map<int, function_state> ac_numkey_event;

/**
 * \brief Sets the action map for numkey events.
 *
 * This function initializes the `ac_numkey_event` map with various actions
 * associated with numkey events.
 */
export void set_action_map() {
    ac_numkey_event[numkey_0] = {activate_function_key, deactivate_function_key};
    ac_numkey_event[numkey_1] = {activate_auto_core, close_program};
    ac_numkey_event[numkey_2] = {activate_word, save_file_and_create_new_file};
    ac_numkey_event[numkey_3] = {activate_iTunes, add_brackets_around_clipboard};
    ac_numkey_event[numkey_4] = {print_Daniel_choice, activate_chrome};
    ac_numkey_event[numkey_5] = {print_Luna_choice, activate_visual};
    ac_numkey_event[numkey_6] = {iTunes_next_song, activate_discord};
    ac_numkey_event[numkey_7] = {print_gpt_message, activate_folder};
    ac_numkey_event[numkey_8] = {print_spotify_songs, activate_firefox};
    ac_numkey_event[numkey_9] = {print_timestamp, print_Lily_choice};
    ac_numkey_event[numkey_star] = {print_Star_choice, print_episode_title};
    ac_numkey_event[numkey_plus] = {print_iTunes_songs, print_next_up_song_list};
    ac_numkey_event[numkey_dot]  = {spotify_next_song, activate_spotify};
    ac_numkey_event[numkey_enter] = {spotify_play_pause, activate_vs_code};
    ac_numkey_event[numkey_dash]  = {sp_switch_player, print_Jose_choice};
    ac_numkey_event[numkey_slash] = {retrieve_and_delete_recycle_bin, remove_iTunes_song};
    ac_numkey_event[play_pause_key] = {iTunes_play_pause, print_Eric_choice};
    ac_numkey_event[home_page_key] = {print_Daniel_42_choice, activate_powershell};
    ac_numkey_event[mail_key] = {print_task_list, print_Katrina_choice};
    ac_numkey_event[calculator_key] = {get_user_sp_queue, print_Tabby_choice};
}

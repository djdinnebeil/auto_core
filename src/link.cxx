module link;
import base;
import config;
import clipboard;
import logger;
import print;
import thread;
import main;
import <Windows.h>;

bool dash_selected = false;

string read_file_to_string() {
    ifstream file(R"(.\link\my_study.txt)");
    if (!file.is_open()) {
        throw runtime_error("Could not open file ");
    }
    ss buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * \brief Formats a dash-separated prompt string.
 *
 * Splits the input string by dashes and formats it for better readability.
 *
 * \param str The input string to format.
 * \return The formatted string.
 */
string format_dash_prompt(const string& str) {
    vector<string> names;
    ss s(str);
    string name;
    char dash = '-';
    while (getline(s, name, dash)) {
        names.push_back(name);
    }
    oss formatted_names;
    for (size_t i = 0; i < names.size(); i++) {
        formatted_names << names[i];
        if ((i + 1) == 35) {
            formatted_names << ", ";
        }
        else if (((i + 1) % 5 == 0) && (i + 1 != names.size())) {
            formatted_names << "";
        }
        else if (i + 1 != names.size()) {
            formatted_names << ", ";
        }
    }
    loggnl("format_dash_prompt: ");
    auto formatted_names_str = formatted_names.str();
    return formatted_names_str;
}

/**
 * \brief Retrieves a GPT message from a file.
 *
 * Reads prompts from a file, formats them if necessary, and presents them for user selection.
 *
 * \return The selected GPT message.
 */
string get_gpt_message() {
    logg("get_gpt_message()");
    const string gpt_path_prompts = R"(.\link\gpt_prompts.rc)";
    ifstream file(gpt_path_prompts);
    if (!file.is_open()) {
        print("error reading file");
        return "";
    }
    vector<string> gpt_prompts;
    string prompt;
    while (getline(file, prompt)) {
        if (prompt.find("*") != string::npos && prompt[0] == '*') {
            prompt = prompt.substr(1);
            gpt_prompts.push_back(prompt);
        }
        else if (prompt.find(" ") == string::npos) {
            prompt = format_dash_prompt(prompt);
            gpt_prompts.push_back(prompt);
        }
        else {
            gpt_prompts.push_back(prompt);
        }
    }
    file.close();
    oss gpt_prompt_choice;
    gpt_prompt_choice << "Enter the number:\n";
    for (size_t i = 0; i < gpt_prompts.size(); ++i) {
        prompt = gpt_prompts[i];
        gpt_prompt_choice << format("{}. for {}\n", i + 1, prompt);
    }
    gpt_prompt_choice << "> ";
    printnl(gpt_prompt_choice.str());
    string selection_str;
    int selection;
    while (true) {
        getline(cin, selection_str);
        logg("{}", selection_str);
        try {
            if (selection_str.empty()) {
                string file_output = read_file_to_string();
                if (file_output.empty()) {
                    return gpt_prompts[gpt_prompts.size() - 1];
                }
                return file_output;
            }
            selection = stoi(selection_str);
            if (selection == 0 || selection > gpt_prompts.size()) {
                printnl("Incorrect input\nEnter again: ");
            }
            else if (selection == 5) {
                dash_selected = true;
                return gpt_prompts[selection - 1];
            }
            else {
                return gpt_prompts[selection - 1];
            }
        }
        catch (const invalid_argument&) {
            printnl("Incorrect input\nEnter again: ");
        }
    }
    logg("end of get_gpt_message()");
}

/**
 * \brief Retrieves and prints a GPT message in a separate thread.
 *
 * This function runs the process of getting and printing a GPT message in a separate thread
 * to avoid blocking the main thread.
 */
void threaded_print_gpt_message() {
    wstring most_recent_clipboard_text = get_clipboard_text();
    dash_selected = false;
    HWND current_window_handle = GetForegroundWindow();
    set_focus_auto_core();
    string gpt_message = get_gpt_message();
    print(gpt_message);
    wss ws;
    ws << gpt_message.c_str();
    //if (dash_selected) {
    //    ws << "\n\n";
    //}
    set_clipboard_text(ws.str());
    SetForegroundWindow(current_window_handle);
    paste_from_clipboard();
    Sleep(100);
    set_clipboard_text(most_recent_clipboard_text);
}

/**
 * \brief Prints a GPT message.
 *
 * Initiates the process of retrieving and printing a GPT message in a separate thread.
 * \runtime
 */
void print_gpt_message() {
    logg("print_gpt_message()");
    thread t([=]() {run_with_exception_handling(threaded_print_gpt_message); });
    t.detach();
}

/**
\file dash.cxx
\brief This script generates the `dash_x.ixx` file for runtime configuration of the Auto Core project.

This script performs the following tasks:
- Extracts the module description comment and function description comments from an existing `dash_x.ixx` file.
- Inserts the `get_numkey_vk_code` function definition into the generated `dash_x.ixx` file.
- Processes the files in the specified directory to extract functions marked with `\runtime` and inserts them into the `get_function_by_name` function map.
- Includes special case handling for the `make_print_choice` function within the `get_function_by_name` function.
- Compiles the Auto Core project using `msbuild` after generating the `dash_x.ixx` file.

This script automates the process of creating and updating the runtime configuration module, ensuring that the necessary functions are correctly mapped and available for runtime use.

Dependencies:
- Requires the `dash_x.ixx` file to be present in the specified path for extracting comments.
- Uses standard libraries for file operations and string manipulation.

Usage:
- Run this script to generate the `dash_x.ixx` file and rebuild the Auto Core project with the updated runtime configuration.
*/
import base;
import logger;
import print;
import <Windows.h>;
import <cstdlib>;

// Path to the dash_x.ixx file
const string dash_x_path = R"(C:\DJ\My Folder\Auto Core\import\dash_x.ixx)";

/**
 * \brief Extracts the module description comment from the dash_x.ixx file.
 * \return The module description comment as a string.
 */
string extract_module_description_comment() {
    string description;
    ifstream file(dash_x_path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << dash_x_path << endl;
        return "";
    }
    string line;
    getline(file, line);
    if (line.starts_with("/*")) {
        description += line + "\n";
        while (getline(file, line)) {
            description += line + "\n";
            if (line.find("*/") != string::npos) {
                break;
            }
        }
    }
    file.close();
    return description;
}

/**
 * \brief Extracts the function description comment for a specific function from the dash_x.ixx file.
 * \param function_name The name of the function to extract the comment for.
 * \return The function description comment as a string.
 */
string extract_function_description_comment(const string& function_name) {
    ifstream file(dash_x_path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << dash_x_path << endl;
        return "";
    }
    string line;
    string comment;
    getline(file, line); // Skip the module description comment
    while (getline(file, line)) {
        comment = "";
        if (line.find("/*") != string::npos) {
            comment += line + "\n";
            while (getline(file, line)) {
                comment += line + "\n";
                if (line.find("*/") != string::npos) {
                    getline(file, line);
                    break;
                }
            }
            if (line.find(function_name) != string::npos) {
                file.close();
                return comment;
            }
            else {
                continue;
            }
        }
    }
    file.close();
    return "";
}

/**
 * \brief Inserts the get_numkey_vk_code function into the runtime file.
 * \param runtime_file The output file stream to write the function to.
 */
void insert_vk_code(ofstream& runtime_file) {
    runtime_file << R"(export int get_numkey_vk_code(string_view vk_code_string) {
    static const unordered_map<string_view, int> numkey_vk_code_map = {
        {"numkey_0", numkey_0},
        {"numkey_1", numkey_1},
        {"numkey_2", numkey_2},
        {"numkey_3", numkey_3},
        {"numkey_4", numkey_4},
        {"numkey_5", numkey_5},
        {"numkey_6", numkey_6},
        {"numkey_7", numkey_7},
        {"numkey_8", numkey_8},
        {"numkey_9", numkey_9},
        {"numkey_star", numkey_star},
        {"numkey_plus", numkey_plus},
        {"numkey_dot", numkey_dot},
        {"numkey_enter", numkey_enter},
        {"numkey_dash", numkey_dash},
        {"numkey_slash", numkey_slash},
        {"play_pause_key", play_pause_key},
        {"calculator_key", calculator_key},
        {"mail_key", mail_key},
        {"home_page_key", home_page_key},
    };
    auto it = numkey_vk_code_map.find(vk_code_string);
    return (it != numkey_vk_code_map.end()) ? it->second : -1;
}
)";
}

/**
 * \brief Inserts the special case handling for the function get_function_by_name.
 * \param runtime_file The output file stream to write the special case handling to.
 */
void insert_special_case_in_function_by_name(ofstream& runtime_file) {
    runtime_file << R"(    // Check for special case "make_print_choice"
    if (function_name.rfind("make_print_choice", 0) == 0) {
        size_t opening_quotation = function_name.find('"');
        size_t closing_quotation = function_name.find("\",");
        string choice_name = string(function_name.substr(opening_quotation + 1, closing_quotation - opening_quotation - 1));
        bool bool_value = function_name.find("true", closing_quotation) != string::npos;
        return make_print_choice(choice_name, bool_value);
    }
)";
}

/**
 * \brief Processes a file to extract functions marked with \runtime and inserts them into the runtime file.
 * \param runtime_file The output file stream to write the functions to.
 * \param filepath The path to the file to process.
 */
void process_file(ofstream& runtime_file, const string& filepath) {
    cout << filepath << endl;
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filepath << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        if (line.find(R"(* \runtime)") != string::npos) {
            cout << "Found \\runtime in: " << endl;
            string function_name;
            do {
                if (line.find(R"(*/)") != string::npos) {
                    cout << "Found */" << endl;
                    string function_line;
                    getline(file, function_line);
                    if (function_line.find("void ") != string::npos) {
                        size_t first_space = function_line.find(" ");
                        size_t parentheses = function_line.find("()");
                        function_name = function_line.substr(first_space + 1, parentheses - first_space - 1);
                        cout << function_name << endl;
                        break;
                    }
                }
            } while (getline(file, line));
            if (!function_name.empty()) {
                runtime_file << "        {\"" << function_name << "\", &" << function_name << "},\n";
            }
        }
    }
    file.close();
}

int main(int argc, char* argv[]) {
    string module_description_comment = extract_module_description_comment();
    string vk_code_function_comment = extract_function_description_comment("get_numkey_vk_code");
    string function_by_name_function_comment = extract_function_description_comment("get_function_by_name");
    ofstream runtime_file(R"(C:\DJ\My Folder\Auto Core\import\dash_x.ixx)");
    if (!runtime_file.is_open()) {
        cerr << "Failed to create file: " << R"(C:\DJ\My Folder\Auto Core\import\dash_x.ixx)" << endl;
        return 1;
    }
    runtime_file << module_description_comment
        << "export module dash_x;\n"
        << "import core;\n\n";
    runtime_file << vk_code_function_comment;
    insert_vk_code(runtime_file);
    runtime_file << "\n";
    runtime_file << function_by_name_function_comment;
    runtime_file << "export function<void()> get_function_by_name(string_view function_name) {\n"
        << "    static const unordered_map<string_view, function<void()>> function_map = {\n";
    string path = R"(C:\DJ\My Folder\Auto Core\import)";
    try {
        if (fs::is_directory(path)) {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    process_file(runtime_file, entry.path().string());
                }
            }
        }
        else {
            cout << "Provided path is not a directory." << endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        cerr << e.what() << endl;
    }
    path = R"(C:\DJ\My Folder\Auto Core\src)";
    try {
        if (fs::is_directory(path)) {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    process_file(runtime_file, entry.path().string());
                }
            }
        }
        else {
            cout << "Provided path is not a directory." << endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        cerr << e.what() << endl;
    }
    runtime_file << "    };\n";
    insert_special_case_in_function_by_name(runtime_file);
    runtime_file << "    // Lookup the function in the map\n"
        << "    auto it = function_map.find(function_name);\n"
        << "    return (it != function_map.end()) ? it->second : nullptr;\n"
        << "}\n";
    runtime_file.close();

    if (argc < 2) {
        cout << "Finished building dash_x" << endl;
    }
    else {
        cout << "\n\nBuild Auto Core? Press any key to continue" << endl;
        cin.get();
        int result = system(R"(msbuild "C:\DJ\My Folder\Auto Core\visual\auto_core.sln" /p:Configuration=Release)");
        if (result != 0) {
            cerr << "Build failed with a return code of " << result << endl;
        }
    }
    cout << "Press any key to exit." << endl;
    cin.get();
    return 0;
}

/**
\file reLoaD-include.cxx
\brief This program scans the 'import' directory for scripts containing the \hardlink tag and creates hard links in the 'include' directory.

The program performs the following steps:
1. Defines the target directory for hard links.
2. Contains a function `create_hard_link` that creates a hard link for a specified file.
3. Contains a function `parse_for_hard_link` that parses a file for the \hardlink tag and creates a hard link if the tag is found.
4. The `main` function scans the 'import' directory, identifies files containing the \hardlink tag, and creates corresponding hard links in the 'include' directory.

## Key Components
- **create_hard_link**: This function generates a hard link for a given file in the specified hard link directory.
- **parse_for_hard_link**: This function reads a file line by line to check for the presence of the \hardlink tag and invokes `create_hard_link` when the tag is found.
- **main**: This function traverses the 'import' directory, processes each file, and creates hard links for files containing the \hardlink tag.

## Dependencies
- Windows API: Uses the `CreateHardLinkA` function from `Windows.h` to create hard links.
- C++ Standard Library: Utilizes libraries such as `<iostream>`, `<fstream>`, `<string>`, `<filesystem>`, and `<cstdlib>`.

## Usage
- Execute this program to scan the 'import' directory for scripts with the \hardlink tag and create corresponding hard links in the 'include' directory.
- Ensure that the 'import' and 'include' directories exist and are correctly specified in the `path` and `hard_link_directory` constants.

## Error Handling
- The program outputs error messages if it fails to open a file or create a hard link.

## Example
Assuming `C:\DJ\My Folder\Auto Core\import\example_script.ixx` contains the \hardlink tag:
1. The program will find this tag.
2. It will create a hard link named `example_script.ixx` in the `C:\DJ\My Folder\Auto Core\include\` directory.

*/
import base;
import print_b;
import <Windows.h>;

const string hard_link_directory = R"(C:\DJ\My Folder\Auto Core\shared\include\)";

/**
 * \brief Creates a hard link to the specified file.
 * \param filepath The path of the file to create a hard link for.
 * \return True if the hard link was created successfully, otherwise false.
 */
bool create_hard_link(const string& filepath) {
    string link_name = "";
    size_t last_backslash_pos = filepath.rfind('\\');
    if (last_backslash_pos != string::npos) {
        link_name = filepath.substr(last_backslash_pos + 1);
    }
    string hard_link_filepath = hard_link_directory + link_name;
    if (CreateHardLinkA(hard_link_filepath.c_str(), filepath.c_str(), NULL)) {
        print("Hard link created successfully.");
        return true;
    }
    else {
        print("Failed to create hard link. Error: {}", GetLastError());
        return false;
    }
}
/**
 * \brief Parses a file for the \hardlink tag and creates a hard link if found.
 * \param filepath The path of the file to parse.
 */
void parse_for_hard_link(const string& filepath) {
    ifstream file(filepath);
    string line;
    while (getline(file, line)) {
        if (line.find(R"(\hardlink)") != string::npos) {
            print("hardlink found in {}", filepath);
            create_hard_link(filepath);
            break;
        }
    }
    file.close();
}

int main() {
    string path = R"(C:\DJ\My Folder\Auto Core\import)";
    fs::create_directory(hard_link_directory);
    try {
        if (fs::is_directory(path)) {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    parse_for_hard_link(entry.path().string());
                }
            }
        }
        else {
            print("Provided path is not a directory.");
        }
    }
    catch (const fs::filesystem_error& e) {
        print("{}", e.what());
    }
    print("Press any key to exit.");
    cin.get();
    return 0;
}
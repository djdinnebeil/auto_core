/**
\file base.ixx
\brief Central module for importing essential C++ Standard Library components and setting up namespace aliases and using directives for widespread use throughout the project.
\details
This module centralizes the import of essential C++ Standard Library components.
In the future, the individual `export import` statements will be replaced with `export import std` for simplicity.
\todo The current structure allows for the potential transition and removal of specific statements when Microsoft upgrades <Windows.h>.
\hardlink
 */
export module base;

// Importing C++ Standard Library components in alphabetical order
export import <array>;
export import <chrono>;
export import <condition_variable>;
export import <filesystem>;
export import <format>;
export import <fstream>;
export import <functional>;
export import <iostream>;
export import <mutex>;
export import <random>;
export import <sstream>;
export import <string>;
export import <thread>;
export import <unordered_map>;
export import <vector>;

export {
    // Chrono Utilties
    namespace chrono = std::chrono;

    // Filesystem Utilities
    namespace fs = std::filesystem;
    using std::ios;
    using std::ios_base;

    // Threading Utilities
    namespace this_thread = std::this_thread;
    using std::thread;
    using std::mutex;
    using std::condition_variable;
    using std::unique_lock;

    // Stream Utilities
    using ss = std::stringstream;
    using iss = std::istringstream;
    using oss = std::ostringstream;
    using wss = std::wostringstream;
    using std::ifstream;
    using std::ofstream;
    using std::streamsize;

    // I/O Stream Objects
    using std::cerr;
    using std::cin;
    using std::cout;
    using std::wcout;
    using std::getline;

    // String Utilities
    using std::string;
    using std::wstring;
    using std::to_string;
    using std::stoi;

    // Formatting and Error Handling
    using std::format;
    using std::format_error;
    using std::invalid_argument;
    using std::vformat;
    using std::make_format_args;
    using std::forward;
    using std::endl;
    using std::runtime_error;

    // STL Containers
    using std::array;
    using std::vector;
    using std::unordered_map;

    // Miscellaneous Utilities
    using std::function;
    using std::setw;
    using std::setfill;
    using std::exception;

    // Random Number Utilities
    using std::mt19937;
    using std::random_device;
    using std::uniform_int_distribution;

    // Type Utilities
    using std::is_same_v;
    using std::decay_t;

    // Tuple Utilities
    using std::make_tuple;
    using std::apply;
}

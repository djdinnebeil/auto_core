module tasks;
import base;
import config;
import clipboard;
import logger;
import print;
import thread;
import main;
import <Windows.h>;

string get_task_list() {
    const string task_list_path = R"(.\link\task_list.rc)";
    ifstream file(task_list_path, ios::binary | ios::ate);
    if (!file.is_open()) {
        print("error reading file");
        return "";
    }
    streamsize size = file.tellg();
    if (size == 0) {
        print_to_screen("Nothing pending today.");
        return "";
    }
    file.seekg(0);
    string task_list = "Today's task list:";
    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        task_list += "\n" + line;
    }
    return task_list;
}

/**
 * \brief Prints the task list.
 *
 * Reads tasks from a file and prints them to the screen. If the file is empty, it indicates
 * that there are no pending tasks for the day.
 * \runtime
 */
void print_task_list() {
    print_to_screen(get_task_list());
}

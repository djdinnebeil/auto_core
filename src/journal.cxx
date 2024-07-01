module journal;
import visual;
import keyboard;
import thread;
import main;
import <Windows.h>;

int upper_choice;
string choice_selector;

/**
 * \brief Generates a random number within a specified range.
 *
 * \param range_low The lower bound of the range.
 * \param range_high The upper bound of the range.
 * \return A random number within the specified range.
 */
int get_random_number(int range_low, int range_high) {
    static mt19937 engine {random_device{}()};
    uniform_int_distribution<int> dist(range_low, range_high);
    return dist(engine);
}

/**
 * \brief Sets the number of choices for the random selection.
 *
 * Prompts the user to enter the number of choices. If the input is invalid or empty,
 * it defaults to 2 choices.
 */
void set_number_of_choices() {
    HWND currentWindowHandle = GetForegroundWindow();
    set_focus_auto_core();
    string choice_selection;
    string choice_number_str;
    printnl("Enter number of choices: ");
    getline(cin, choice_number_str);
    if (choice_number_str.empty()) {
        upper_choice = 2;
        logg("");
    }
    else {
        logg("{}", choice_number_str);
        try {
            int choice_number = stoi(choice_number_str);
            if (choice_number >= 2) {
                upper_choice = choice_number;
            }
            else if (choice_number == 1 || choice_number == 0) {
                upper_choice = 1;
            }
        }
        catch (...) {
            upper_choice = 2;
        }
    }
    SetForegroundWindow(currentWindowHandle);
}

/**
 * \brief Generates and prints a random choice with a name.
 *
 * Generates a random number within the range of 1 to upper_choice and prints it to the screen.
 */
void thread_print_choice_with_name() {
    set_number_of_choices();
    int random_choice_number = get_random_number(1, upper_choice);
    print_to_screen(format("{} selects {}.", choice_selector, random_choice_number));
}

/**
 * \brief Generates and prints a random choice with zero included.
 *
 * Generates a random number within the range of 0 to upper_choice and prints it to the screen.
 */
void thread_print_choice_with_zero() {
    set_number_of_choices();
    int random_choice_number = get_random_number(0, upper_choice);
    print_to_screen(format("{} selects {}.", choice_selector, random_choice_number));
}

/**
 * \brief Prints a random choice for a given name.
 *
 * \param name The name associated with the choice.
 * \param include_zero Indicates if the range should include zero.
 */
void print_choice(const string& name, bool include_zero) {
    choice_selector = name;
    if (include_zero) {
        thread t([=]() {run_with_exception_handling(thread_print_choice_with_zero); });
        t.detach();
    }
    else {
        thread t([=]() {run_with_exception_handling(thread_print_choice_with_name); });
        t.detach();
    }
}

/**
 * \brief Creates a function that prints a random choice for a given name.
 *
 * \param name The name associated with the choice.
 * \param include_zero Indicates if the range should include zero.
 * \return A function that prints the random choice.
 */
function<void()> make_print_choice(const string& name, bool include_zero) {
    return [=]() {print_choice(name, include_zero); };
}

/** \runtime */
void print_Tabby_choice() {
    choice_selector = "Tabby";
    thread t([=]() {run_with_exception_handling(thread_print_choice_with_zero); });
    t.detach();
}

/** \runtime */
void print_Eric_choice() {
    choice_selector = "Eric";
    thread t([=]() {run_with_exception_handling(thread_print_choice_with_zero); });
    t.detach();
}

/** \runtime */
void print_Katrina_choice() {
    choice_selector = "Katrina";
    thread t([=]() {run_with_exception_handling(thread_print_choice_with_zero); });
    t.detach();
}

/** \runtime */
void print_Lily_choice() {
    choice_selector = "Lily";
    thread t([=]() {run_with_exception_handling(thread_print_choice_with_name); });
    t.detach();
}

/** \runtime */
void print_Star_choice() {
    choice_selector = "Star";
    thread t([=]() {run_with_exception_handling(thread_print_choice_with_zero); });
    t.detach();
}

/** \runtime */
void print_Luna_choice() {
    choice_selector = "Luna";
    thread t([=]() {run_with_exception_handling(thread_print_choice_with_name); });
    t.detach();
}

/** \runtime */
void print_Daniel_42_choice() {
    int random_number = get_random_number(0, 100);
    string random_number_str = format("Daniel selects {}.", random_number);
    print_to_screen(random_number_str);
}

/** \runtime */
void print_Daniel_choice() {
    choice_selector = "Daniel";
    thread t([=]() {run_with_exception_handling(thread_print_choice_with_name); });
    t.detach();
}

/** \runtime */
void print_Jose_choice() {
    choice_selector = "Jose";
    thread t([=]() {run_with_exception_handling(thread_print_choice_with_zero); });
    t.detach();
}

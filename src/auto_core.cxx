/**
 * \file auto_core.cxx
 * \brief Main entry point for the Auto Core application.
 *
 * This file contains the primary logic for initializing and running the Auto Core application.
 * It includes functions for setting up the console, initializing core components, creating
 * named pipe servers for inter-process communication, and starting necessary components.
 * The main function manages the application lifecycle, including setting up the environment,
 * entering the message loop, and handling exceptions.
 *
 * \author Jose, Star, DJ, Daniel, Tabby
 */
module auto_core;
import config;
import journey;
import runtime;
import dash;
import end;
import <Windows.h>;

/**
 * \brief Initializes the console and sets up exception handling.
 */
void auto_init() {
    SetConsoleOutputCP(CP_UTF8);
    SetUnhandledExceptionFilter(unhandled_exception_handler);
    SetConsoleCtrlHandler(console_close_event, TRUE);
}
/**
 * \brief Initializes the core components, sets up keyboard hooks, and configures the program window.
 */
void core_init() {
    keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, send_numpad_event, NULL, 0);
    main_thread_id = GetCurrentThreadId();
    program_window = GetConsoleWindow();
    close_window = close_window_hidden_init();
}
/**
 * \brief Creates named pipe servers for inter-process communication.
 */
void create_pipe_servers() {
    ac_itunes_pipe = create_pipe_server(L"ac_itunes_pipe");
    ac_sp_pipe = create_pipe_server(L"ac_sp_pipe");
    wake_pipe = create_pipe_server(L"wake_pipe");
}

/**
 * \brief Starts the components required for the application.
 */
void start_components() {
    start_iTunes_component();
    start_sp_component();
    start_wake_component();
    if (config.start_server) {
        start_server();
    }
}
void print_program_ready() {
    string program_start_str = "Program ready";
    program_start_str += "\nToday is " + get_day_of_week();
    program_start_str += "\n" + get_task_list();
    cout << program_start_str << endl;
    Sleep(350);
    logg(program_start_str);
}

/**
 * \brief The main function of the application.
 *
 * This function initializes the application, sets up the environment, and enters the message loop.
 *
 * \return int The exit code of the application.
 */
int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "child") == 0) {
        auto_init();
        core_init();
        log_init();
        crash_check();
        create_pipe_servers();
        start_components();
        config.runtime_enabled ? parse_and_set_action_map() : set_action_map();
        // Benchmark using std::chrono for portable, high-level timing.
// Measures average setup time for parse_and_set_action_map() or set_action_map()
// based on config.runtime_enabled. Useful for cross-platform validation.
// import <chrono>
        {
            using Clock = chrono::high_resolution_clock;
            double total = 0;
            const int N = 1000;
            for (int i = 0; i < N; ++i) {
                auto start = Clock::now();
                config.runtime_enabled ? parse_and_set_action_map() : set_action_map();
                auto end = Clock::now();
                total += chrono::duration_cast<chrono::microseconds>(end - start).count();
            }
            print("Average setup time for runtime_enabled={} over {} runs: {} microseconds", config.runtime_enabled, N, total / N);
        }

        // Benchmark using QueryPerformanceCounter for high-resolution Windows-native timing.
        // Measures the same setup function and converts result to microseconds.
        // The result is formatted with precision and length control for display.
        // import <Windows.h>
        {
            LARGE_INTEGER frequency, start_time, end_time;
            QueryPerformanceFrequency(&frequency);

            const int NUM_RUNS = 1000;
            double total_time = 0;

            for (int i = 0; i < NUM_RUNS; ++i) {
                QueryPerformanceCounter(&start_time);
                config.runtime_enabled ? parse_and_set_action_map() : set_action_map();
                QueryPerformanceCounter(&end_time);

                total_time += static_cast<double>(end_time.QuadPart - start_time.QuadPart) * 1'000'000.0 / frequency.QuadPart;
            }

            double avg_time = total_time / NUM_RUNS;

            oss avg_time_oss;
            avg_time_oss << std::fixed << std::setprecision(3) << avg_time;
            string avg_time_str = avg_time_oss.str();

            if (avg_time_str.size() > 8) {
                avg_time_str = avg_time_str.substr(0, 8);
            }
            print("Average setup time for runtime_enabled={} over {} runs: {} microseconds", config.runtime_enabled, NUM_RUNS, avg_time_str);
        }

        thread taskbar_ps_thread(run_taskbar_ps);
        taskbar_ps_thread.detach();
        thread program_ready_thread(print_program_ready);
        program_ready_thread.detach();
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0) > 0) {
            try {process_numpad_event(msg);}
            catch (const exception& e) {print("caught exception: {}", e.what());}
            catch (...) {print("uncaught exception");}
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(50);
        return 0;
    }
    else {
        log_start();
        start_journey();
        log_end();
        return 0;
    }
}

/*
There are two tests that can be copied and pasted into main().

Note that there is a print statement to notify the user if debugger mode is enabled.
*/


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
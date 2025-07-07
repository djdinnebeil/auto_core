/**
 * \file journey.ixx
 * \brief Provides functions to create processes and start a journey process.
 *
 * This module includes functions for creating processes and starting a specific journey process.
 */
export module journey;
import base;
import print;
import <Windows.h>;

export {
    bool create_process(const wstring& path);
    void start_journey();
}

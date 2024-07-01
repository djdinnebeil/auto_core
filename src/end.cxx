module end;
import base;
import config;
import logger;
import print;
import numkey;
import main;
import runtime;
import <Windows.h>;

/**
 * \brief Window procedure for handling close events.
 *
 * This function processes messages sent to the hidden window. It handles
 * WM_QUERYENDSESSION and WM_ENDSESSION messages to detect system shutdown events.
 *
 * \param hWnd Handle to the window.
 * \param message The message code.
 * \param wParam Additional message information.
 * \param lParam Additional message information.
 * \return The result of the message processing.
 */
LRESULT CALLBACK close_procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_QUERYENDSESSION:
        logg("close_procedure() - WM_QUERYENDSESSION");
        return TRUE;
    case WM_ENDSESSION:
        if (wParam == TRUE) {
            logg("close_procedure() - WM_ENDSESSION");
            primary = false;
            PostThreadMessage(main_thread_id, ac_numkey_1, 0, numkey_1);
            close_program();
            return TRUE;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

/**
 * \brief Initializes a hidden window to detect close events.
 *
 * This function creates a hidden window with a custom window procedure to
 * handle system shutdown events.
 *
 * \return Handle to the created hidden window.
 */
HWND close_window_hidden_init() {
    const wchar_t CLASS_NAME[] = L"Auto Core Close Window Hidden Class";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = close_procedure;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    RegisterClassW(&wc);
    HWND hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Auto Core Close Window Hidden",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );
    return hWnd;
}


/**
 * \brief Handles console close events.
 *
 * This function processes console close events such as CTRL_CLOSE_EVENT,
 * CTRL_BREAK_EVENT, and CTRL_C_EVENT. It ensures that the program can respond
 * appropriately by logging the event and posting a message to the main thread.
 *
 * \param dwType The type of control signal received.
 * \return TRUE if the event was handled, FALSE otherwise.
 */
BOOL WINAPI console_close_event(DWORD dwType) {
    if (!program_closing) {
        switch (dwType) {
        case CTRL_CLOSE_EVENT:
            logg("console_close_event() - CTRL_CLOSE_EVENT");
            primary = false;
            PostThreadMessage(main_thread_id, ac_numkey_1, 0, numkey_1);
            return TRUE;
        case CTRL_BREAK_EVENT:
            logg("console_close_event() - CTRL_BREAK_EVENT");
            primary = false;
            PostThreadMessage(main_thread_id, ac_numkey_1, 0, numkey_1);
            return TRUE;
        case CTRL_C_EVENT:
            logg("console_close_event() - CTRL_C_EVENT");
            primary = false;
            PostThreadMessage(main_thread_id, ac_numkey_1, 0, numkey_1);
            return TRUE;
        default:
            logg("console_close_event() - default");
            break;
        }
    }
    return FALSE;
}

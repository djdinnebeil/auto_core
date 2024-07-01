module clipboard;
import base;
import config;
import logger;
import <Windows.h>;

/**
 * \brief Sets the text content of the clipboard.
 * \param text The text to be set in the clipboard.
 */
void set_clipboard_text(const wstring& text) {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hClipboardData;
        size_t size = (text.size() + 1) * sizeof(wchar_t);
        hClipboardData = GlobalAlloc(GMEM_MOVEABLE, size);
        if (hClipboardData != nullptr) {
            wchar_t* pchData = static_cast<wchar_t*>(GlobalLock(hClipboardData));
            if (pchData != nullptr) {
                memcpy(pchData, text.c_str(), size);
                GlobalUnlock(hClipboardData);
                SetClipboardData(CF_UNICODETEXT, hClipboardData);
            }
        }
        CloseClipboard();
    }
}

/**
 * \brief Retrieves the text content from the clipboard.
 * \return The text content of the clipboard.
 */
wstring get_clipboard_text() {
    if (OpenClipboard(nullptr)) {
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData == nullptr) {
            logg("No text found in clipboard");
            cout << "No text found in clipboard" << endl;
            CloseClipboard();
            return L"";
        }
        wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
        if (pszText == nullptr) {
            logg("Failed to lock clipboard data");
            cout << "Failed to lock clipboard data" << endl;
            return L"";
        }
        wstring text(pszText);
        GlobalUnlock(hData);
        CloseClipboard();
        return text;
    }
    else {
        return L"";
    }
}

/**
* \brief Simulates pressing 'Ctrl + V' to paste text from the clipboard.
*/
void paste_from_clipboard() {
    INPUT inputs[4] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'V';
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'V';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

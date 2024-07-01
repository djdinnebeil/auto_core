module keyboard;
import base;
import <Windows.h>;

/**
 * \brief Sends a single key press and release.
 * \param vk_code Virtual key code of the key to be pressed and released.
 */
void send_single_key(BYTE vk_code) {
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = vk_code;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = vk_code;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

/**
 * \brief Sends a key combination press and release.
 * \param keys Array of virtual key codes to be pressed and released.
 * \param key_count Number of keys in the array.
 */
void send_key_combination(const BYTE* keys, int key_count) {
    int total_inputs = key_count * 2;
    INPUT* inputs = new INPUT[total_inputs];
    ZeroMemory(inputs, total_inputs * sizeof(INPUT));
    for (int i = 0; i < key_count; ++i) {
        inputs[i].type = INPUT_KEYBOARD;
        inputs[i].ki.wVk = keys[i];
        inputs[i + key_count].type = INPUT_KEYBOARD;
        inputs[i + key_count].ki.wVk = keys[i];
        inputs[i + key_count].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    SendInput(total_inputs, inputs, sizeof(INPUT));
    delete[] inputs;
}

/**
 * \brief Press and release the Windows key with a specified number key.
 * \param position The number key to be pressed with the Windows key (0-9).
 */
void send_winkey(int position) {
    BYTE keys[2] = {VK_RWIN, static_cast<BYTE>(0x30 + position)};
    send_key_combination(keys, 2);
}

/**
 * \brief Press and release a specified number key.
 * \param number The number key to be pressed and released (0-9).
 */
void send_winkey_and_number(int number) {
    send_single_key(static_cast<BYTE>(0x30 + number));
}

/**
 * \brief Press and hold the Windows key.
 */
void press_and_hold_winkey() {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VK_RWIN;
    SendInput(1, &input, sizeof(INPUT));
}

/**
 * \brief Release the Windows key.
 */
void release_winkey() {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VK_RWIN;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}


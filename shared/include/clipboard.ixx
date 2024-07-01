/**
\file clipboard.ixx
\brief Facilitates clipboard interactions for string handling within the Auto Core system.

Auto Core streamlines text insertion by utilizing the system clipboard and the 'ctrl + v' paste shortcut to send text to the active textbox.
\hardlink
*/
module;

#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

export module clipboard;
import base;
import config;
import logger;
import <Windows.h>;

export {
	DLL_API void set_clipboard_text(const wstring& text);
	DLL_API wstring get_clipboard_text();
	DLL_API void paste_from_clipboard();
}
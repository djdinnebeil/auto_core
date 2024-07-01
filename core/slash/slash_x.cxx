/**
\file slash.cxx
\brief Handles the retrieval and deletion of Recycle Bin contents.

This module scans the Recycle Bin to gather detailed information about its contents, including files, folders, ZIP files, and music files. It retrieves metadata for music files using TagLib and organizes the data into separate categories. The contents are then copied to the clipboard and displayed. Finally, it empties the Recycle Bin.

The main function is designed to handle any exceptions that may occur during the process, ensuring robust error handling and proper cleanup.
*/
import base;
import config;
import clipboard;
import logger;
import print;
import utils;
import <atlbase.h>;
import <shlobj.h>;
import <shlwapi.h>;
#pragma warning(disable : 4251)
import <taglib/fileref.h>;
import <taglib/tag.h>;

#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Shlwapi.lib")

/**
 * \brief Extracts the folder name from a given file path.
 * \param filepath The full file path.
 * \return The folder name extracted from the file path.
 */
wstring extract_folder_name(const wstring& filepath) {
    size_t backslash_index = filepath.find_last_of(L'\\');
    if (backslash_index == wstring::npos) {
        return filepath;
    }
    return filepath.substr(backslash_index + 1);
}
/**
 * \brief Extracts the ZIP file name from a given file path.
 * \param filepath The full file path.
 * \return The ZIP file name extracted from the file path.
 */
wstring extract_zip_file_name(const wstring& filepath) {
    size_t backslash_index = filepath.find_last_of(L'\\');
    if (backslash_index == wstring::npos) {
        return filepath;
    }
    return filepath.substr(backslash_index + 1);
}
/**
 * \brief Extracts the file extension in lowercase from a given file path.
 * \param filepath The full file path.
 * \return The lowercase file extension.
 */
wstring extract_extension_type_lowercase(const wstring& filepath) {
    size_t ext_dot = filepath.find_last_of(L'.');
    if (ext_dot == wstring::npos) {
        return filepath;
    }
    wss lower_case_ext;
    for (wchar_t c : filepath.substr(ext_dot)) {
        lower_case_ext << static_cast<wchar_t>(tolower(c));
    }
    return lower_case_ext.str();
}
/**
 * \brief Extracts the file name without extension from a given file path.
 * \param filepath The full file path.
 * \return The file name without extension.
 */
wstring extract_file_name(const wstring& filepath) {
    size_t backslash = filepath.find_last_of(L'\\');
    if (backslash == wstring::npos) {
        return filepath;
    }
    wstring filename = filepath.substr(backslash + 1);
    size_t ext_dot = filename.find_last_of(L'.');
    if (ext_dot == wstring::npos || ext_dot == backslash + 1) {
        return filename;
    }
    return filename.substr(0, ext_dot);
}
/**
 * \brief Extracts the artist name from a given file path.
 * \param fullPath The full file path.
 * \return The artist name extracted from the file path.
 */
wstring extract_artist(const wstring& fullPath) {
    size_t lastBackslashIndex = fullPath.rfind(L'\\');
    if (lastBackslashIndex == wstring::npos) {
        return L"";
    }
    size_t secondLastBackslashIndex = fullPath.rfind(L'\\', lastBackslashIndex - 1);
    if (secondLastBackslashIndex == wstring::npos) {
        return L"";
    }
    size_t thirdLastBackslashIndex = fullPath.rfind(L'\\', secondLastBackslashIndex - 1);
    if (thirdLastBackslashIndex == wstring::npos) {
        return L"";
    }
    return fullPath.substr(thirdLastBackslashIndex + 1, secondLastBackslashIndex - thirdLastBackslashIndex - 1);
}
/**
 * \brief Extracts the album name from a given file path.
 * \param fullPath The full file path.
 * \return The album name extracted from the file path.
 */
wstring extract_album(const wstring& fullPath) {
    size_t lastBackslashIndex = fullPath.rfind(L'\\');
    if (lastBackslashIndex == wstring::npos) {
        return L"";
    }
    size_t secondLastBackslashIndex = fullPath.rfind(L'\\', lastBackslashIndex - 1);
    if (secondLastBackslashIndex == wstring::npos) {
        return L"";
    }
    return fullPath.substr(secondLastBackslashIndex + 1, lastBackslashIndex - secondLastBackslashIndex - 1);
}
/**
 * \brief Processes a music file and retrieves its metadata.
 * \param recycled_file A TagLib::FileRef object representing the recycled file.
 * \param filepath The full file path.
 * \return A formatted string with the music file's metadata.
 */
wstring process_music_file(TagLib::FileRef recycled_file, const wstring& filepath) {
    wstring name = L"";
    wstring artist = L"";
    wstring album = L"";
    wstring dur_str = L"";
    if (!recycled_file.isNull() && recycled_file.tag()) {
        TagLib::Tag* tag = recycled_file.tag();
        name = tag->title().toCWString();
        artist = tag->artist().toCWString();
        album = tag->album().toCWString();
        int duration = recycled_file.audioProperties()->lengthInSeconds();
        wss dur;
        dur << (duration / 60) << ":" << setw(2) << setfill(L'0') << (duration % 60);
        dur_str = dur.str();
    }
    if (name.empty()) {
        name = extract_file_name(filepath);
    }
    if (artist.empty()) {
        artist = extract_artist(filepath);
    }
    if (album.empty()) {
        artist = extract_album(filepath);
    }
    if (dur_str.empty()) {
        dur_str = L"[99:99]";
    }
    else if (dur_str == L"0:00") {
        dur_str = L"[[00:00]]";
    }
    wss ws;
    ws << "[" << name << "] [" << artist << "] [" << album << "] [" << dur_str << "]\n";
    return ws.str();
}
/**
 * \brief Retrieves and deletes the contents of the Recycle Bin.
 */
void retrieve_and_delete_recycle_bin() {
    HRESULT hr_slash = CoInitialize(NULL);
    wss recycle_bin_contents;
    wss recycle_bin_filenames;
    wss recycle_bin_folders;
    wss recycle_bin_zip_files;
    wss music_filenames;
    bool files_detected = false;
    bool folders_detected = false;
    bool zip_files_detected = false;
    bool music_files_detected = false;
    recycle_bin_contents << L"Recycle bin contents:\n";
    recycle_bin_filenames << L"---Files---\n";
    recycle_bin_folders << L"---Folders---\n";
    recycle_bin_zip_files << L"---Zip files---\n";
    music_filenames << L"---Music files---\n";
    CComPtr<IShellFolder> p_desktop_folder;
    if (SUCCEEDED(SHGetDesktopFolder(&p_desktop_folder))) {
        LPITEMIDLIST recycle_bin_pidl;
        if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_BITBUCKET, &recycle_bin_pidl))) {
            CComPtr<IShellFolder> p_recycle_bin_folder;
            if (SUCCEEDED(p_desktop_folder->BindToObject(recycle_bin_pidl, NULL, IID_IShellFolder, (void**)&p_recycle_bin_folder))) {
                CComPtr<IEnumIDList> p_enum;
                if (SUCCEEDED(p_recycle_bin_folder->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &p_enum))) {
                    LPITEMIDLIST p_item;
                    while (p_enum->Next(1, &p_item, NULL) != S_FALSE) {
                        wstring filepath;
                        STRRET str;
                        if (SUCCEEDED(p_recycle_bin_folder->GetDisplayNameOf(p_item, SHGDN_NORMAL, &str))) {
                            WCHAR sz_display_name[MAX_PATH];
                            StrRetToBufW(&str, p_item, sz_display_name, MAX_PATH);
                            filepath = sz_display_name;
                            recycle_bin_contents << filepath << L'\n';
                            SFGAOF attribs = SFGAO_FOLDER;
                            HRESULT hrAttrib = p_recycle_bin_folder->GetAttributesOf(1, const_cast<LPCITEMIDLIST*>(&p_item), &attribs);
                            if (SUCCEEDED(hrAttrib)) {
                                if (attribs & SFGAO_FOLDER) {
                                    size_t dot_pos = filepath.find_last_of(L'.');
                                    if (dot_pos == wstring::npos) {
                                        folders_detected = true;
                                        wstring folder_name = extract_folder_name(filepath);
                                        recycle_bin_folders << folder_name << L'\n';
                                    }
                                    else {
                                        wstring ext = extract_extension_type_lowercase(filepath);
                                        if (ext == L".rar" || ext == L".zip" || ext == L".7z" || ext == L".tar" || ext == L".gz" || ext == L".bz2" || ext == L".cab") {
                                            zip_files_detected = true;
                                            recycle_bin_zip_files << extract_file_name(filepath) << L'\n';
                                        }
                                        else {
                                            folders_detected = true;
                                            wstring folder_name = extract_folder_name(filepath);
                                            recycle_bin_folders << folder_name << L'\n';
                                        }
                                    }
                                }
                                else {
                                    wstring ext = extract_extension_type_lowercase(filepath);
                                    if (ext == L".mp3" || ext == L".m4a") {
                                        music_files_detected = true;
                                        if (filepath.ends_with(L".MP3")) {
                                            music_filenames << L"Bonus found: ";
                                        }
                                        if (SUCCEEDED(p_recycle_bin_folder->GetDisplayNameOf(p_item, SHGDN_FORPARSING, &str))) {
                                            TCHAR sz_file_path[MAX_PATH];
                                            StrRetToBuf(&str, p_item, sz_file_path, MAX_PATH);
                                            TagLib::FileRef recycled_file(sz_file_path);
                                            music_filenames << process_music_file(recycled_file, filepath);
                                        }
                                    }
                                    files_detected = true;
                                    wstring filename = extract_file_name(filepath);
                                    if (filename == L"") {
                                        filename = extract_folder_name(filepath);
                                    }
                                    recycle_bin_filenames << filename << L'\n'; 
                                }
                            }
                        }
                        CoTaskMemFree(p_item);
                    }
                }
            }
            CoTaskMemFree(recycle_bin_pidl);
        }
    }
    CoUninitialize();
    HRESULT hrEmpty = SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
    wss output;
    if (files_detected || folders_detected || zip_files_detected || music_files_detected) {
        output << recycle_bin_contents.str();
        if (files_detected) {
            output << recycle_bin_filenames.str();
        }
        if (folders_detected) {
            output << recycle_bin_folders.str();
        }
        if (zip_files_detected) {
            output << recycle_bin_zip_files.str();
        }
        if (music_files_detected) {
            output << music_filenames.str();
        }
        output << L'\n';
    }
    else {
        output << L"\n\n";
    }
    set_clipboard_text(output.str());
    printnl(output.str());
    paste_from_clipboard();
}

/**
 * \brief Main function for the slash component.
 * Retrieves and deletes the contents of the Recycle Bin, and handles exceptions.
 *
 * \return Exit code of the process.
 */
int main() {
    update_main_log_file();
    try {retrieve_and_delete_recycle_bin();}
    catch (const exception& e) {print("caught exception: {}\n", e.what());}
    catch (...) {print("uncaught exception\n");}
    close_main_log_file();
    return 0;
}
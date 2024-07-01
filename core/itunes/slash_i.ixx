/**
\file slash_i.ixx
\brief Provides functionality to delete a track from iTunes and manage file operations.

This module includes functions to remove a track from iTunes, move the track file to the recycle bin,
or permanently delete the track file.
 */
export module slash_i;
import visual;
import itunes_c;
import itunes_x;
import <shobjidl.h>;
import <ShlObj.h>;

export void remove_iTunes_song();

/**
 * \brief Removes the current track from iTunes.
 *
 * This function deletes the current track from iTunes and moves it to the recycle bin.
 */
void iTunes::remove_track() {
    DISPID dispidDelete;
    const OLECHAR* szDelete = L"Delete";
    BSTR bstrDelete = SysAllocString(szDelete);
    hr = p_current_track->GetIDsOfNames(IID_NULL, &bstrDelete, 1, LOCALE_USER_DEFAULT, &dispidDelete);
    SysFreeString(bstrDelete);
    DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
    hr = p_current_track->Invoke(dispidDelete, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparamsNoArgs, NULL, NULL, NULL);
    p_current_track = nullptr;
    recycle_bin_track();
}
/**
 * \brief Moves the track file to the recycle bin.
 *
 * This function uses the SHFileOperation API to move the track file to the recycle bin and logs the operation.
 */
void iTunes::recycle_bin_track() {
    SHFILEOPSTRUCT shFileOp = {0};
    shFileOp.wFunc = FO_DELETE;
    wstring doubleNullTermPath = track_location + L'\0';
    shFileOp.pFrom = doubleNullTermPath.c_str();
    shFileOp.fFlags = FOF_ALLOWUNDO | FOF_NO_UI;
    wstring logg_message;
    if (SHFileOperation(&shFileOp) != 0) {
        logg_message = L"Error - file not moved to the recycle bin: " + track_location;
    }
    else {
        logg_message = L"File moved to the recycle bin: " + track_location;
    }
    iTunes_logger.logg_and_logg(logg_message);
}
/**
 * \brief Permanently deletes the track file.
 *
 * This function deletes the track file and logs the operation.
 */
void iTunes::delete_track() {
    wstring logg_message;
    if (!DeleteFile(track_location.c_str())) {
        logg_message = L"Error - file not deleted: " + track_location;
    }
    else {
        logg_message = L"File deleted: " + track_location;
    }
    iTunes_logger.logg_and_logg(logg_message);
}

/**
 * \brief Removes the current song from iTunes.
 *
 * This function logs the removal of the current song, retrieves the current track, and calls the remove_track function.
 *
 * \runtime
 */
void remove_iTunes_song() {
    iTunes_logger.logg_and_logg("remove_iTunes_song()");
    ac_iTunes.get_current_track();
    if (ac_iTunes.p_current_track) {
        ac_iTunes.remove_track();
    }
}
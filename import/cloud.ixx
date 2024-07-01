/**
\file cloud.ixx
\brief This connects to a Google Cloud server.

\details
This module facilitates interactions with a Google Cloud server, specifically a Firebase database.
Strings retrieved from Firebase have quotation marks added, and strings sent to Firebase must have quotation marks added.
*/
export module cloud;
import base;
import config;
import visual;
import <cpr/cpr.h>;

export {
    void get_string_from_firebase();
    void update_string_in_firebase(const string& value);
}

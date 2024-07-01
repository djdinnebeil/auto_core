/**
 * \file slash.ixx
 * \brief Manages the emptying of the Recycle Bin.
 *
 * This module provides functionality to retrieve and delete the contents of the Recycle Bin.
 * It utilizes an external executable to perform the deletion.
 */
export module slash;
import base;
import config;
import logger;
import print;
import clipboard;
import utils;
import <Windows.h>;

export void retrieve_and_delete_recycle_bin();

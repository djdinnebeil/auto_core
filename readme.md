# Auto Core for Windows 11
Auto Core is a C++ utility designed to streamline task execution on Windows 11 systems. Auto Core is a specialized keyboard manager for the numpad keys, providing support for quick taskbar access, music player integration, journaling inspiration and helper, and automating text insertion.

## Main Features and Core Architecture
- **Numpad Keyboard Manager:** Monitors and intercepts numpad and additional keys to carry out tasks.
- **System Console Window:** Uses the system console to display output or request input.
- **Automated Text Insertion:** Automates text insertion into the active textbox by utilizing the system clipboard and 'ctrl + v' paste shortcut.
- **Taskbar PowerShell:** Simplifies quick taskbar access by sending keyboard events for the winkey shortcuts (winkey + #0-9) and provides additional support for further taskbar productivity.
- **Music Player Integration:** Supports music player integration for iTunes and Spotify.
- **Journaling Support:** Automates titling and file creation of journaling documents.
- **Creative Writing Inspiration:** Follows the 'dice roll' method to generate random numbers or prompts for journaling inspiration.
- **Browser-based Local File Management:** Runs a local server to facilitate browser-based local file access.
- **System Maintenance:** Supports maintenance tasks, including emptying the recycle bin and logging system wake events.

## Project Architecture
Auto Core is designed with a modular component architecture that separates the main system controller from specialized component executables.

The components are stored in the 'core' directory.

The components are:
- dash_x - for runtime configuration. 
- itunes - for interacting with iTunes.
- server - for a local server to allow local file access through a browser.
- slash - for deleting files and folders
- sp - for interacting with Spotify.
- wake - for tracking system activity, such as waking up.

To differentiate component scripts from main scripts, the following notation is used:
- _x - this indicates an external component
- _c - this indicates a class for the component
- _t - this indicates a thread for the component

The main component will have:
sp.ixx
while the external component will have:
sp_x.ixx

The components talk to each other through pipes.

Modules that are shared between components are marked by adding a \hardlink tag.

## Folder Structure
- assets - Graphical resources.
- bin - Compiled executables.
- build - CMakeLists.txt.
- config - Configuration files.
- core - Core components.
- dash - Runtime configuration.
- dist - Dedicated folder for building.
- docs - Documentation.
- import - Main program modules.
- lib - External libraries.
- link - User-custom strings.
- log - Debugging and operational logging.
- server - Local server setup files.
- shared - Project-built shared libraries and shared modules.
- src - Source code files.
- star - User-specific data like login tokens and journal databases.
- sun - Taskbar shortcut.
- tests - Testing scripts.
- tools - Utilities for expanding functionality.
- utils - Utility and helper scripts, such as creating a new journaling database.
- visual - Visual Studio-related files, such as .sln and .pdb.

## Config Folder

### Summary

The `config` folder holds all runtime and environment settings for Auto Core, while the `dash` folder provides a developer-facing overlay to support editing, debugging, and IntelliSense integration in VS Code.

### Files
- `clock.ini` — Defines `end_of_day`, such as `00:59` vs. `24:59`.
- `logger.ini` — Enables enhanced debugging by forwarding all log statements to the console window.
- `runtime.ini` — Controls whether runtime configuration is enabled. Also allows selection of logging mode: `debug`, `buffered`, or `silent`.
- `runtime_map.ini` — Stores runtime key mappings used when runtime configuration is enabled. If disabled, mappings are hardcoded and this file is ignored.
- `server.ini` — Contains local web server settings, such as the port number, if local file serving is needed.
- `star.ini` — Stores journal-related settings.
- `taskbar.ini` — Lists the first 10 programs pinned to the user's taskbar.

## Dash Folder

The Dash folder acts as a convenience layer to simplify modifying runtime mappings.

- `dash_x.ini` — A hard link to `dash_x.ixx`. Contains a list of functions tagged with `\runtime`. When opened in VS Code, it makes those functions globally accessible, enabling IntelliSense autocomplete.
- `runtime_map.ini` — A hard link to `config/runtime_map.ini`. Contains the actual key-to-function mappings defined by the user.

This setup enables VS Code to provide autocomplete for user-defined runtime functions by simply opening `dash_x.ini`, while linking directly to the active configuration via `runtime_map.ini`.


## Tagging Runtime Functions
To make a function available for runtime configuration, tag it with \runtime.

After tagging, run dash_x.exe to update the function list. This ensures that the new function is recognized and available for use in runtime_map.ini.

Note: Runtime configuration must be enabled in config/runtime.ini for these functions to take effect.

## Modules Overview
For a detailed description of each module, please refer to the [Doxygen documentation](./docs/html/files.html).
## Requirements
- Windows 11
- C++ Compiler (supporting C++23)
- Development environment (e.g., Visual Studio)
- Basic knowledge of C++

## Installation and Setup
Installation and usage of this software require knowledge of C++. Please note that this project has not yet been developed for distribution.

## Changelog
Auto Core originally began as a Python project named Auto Song. This name was inspired by the primary use case for formatting the currently playing song.

When the project was ported to C++, the program name was changed to Auto Core to reflect the greater level of system control and precision-tuning offered by C++.

With the upgrade to C++23 modules, Auto Core branched off into two projects: the Auto Core project and the Visual Core project. The Auto Core project is focused on improving and fine-tuning the program core, while the Visual Core project is dedicated to developing and optimizing the separate components, such as music player or cloud components.

## License
License © 2024 DJ, Daniel

Anyone is free to use, copy, and distribute Auto Core. This software is expected to be modified and configured by the individual user. Contributions to the Auto Core project can be made through the Visual Core project.

Disclaimer: This software is dependent on the proper functionality and configuration of Windows 11. The developers disclaim any responsibility for failures, inaccuracies, or other issues that may arise from malfunctions or misconfigurations of Windows 11.

## Acknowledgements
I appreciate the love and joy from Jose, Star, and Tabby! And, Lily, Katrina, and Eric, too! And James, Tyler, and JJ!

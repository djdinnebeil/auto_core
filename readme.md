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

| Component | Purpose                                 | Executable Name | Notes |
|-----------|-----------------------------------------|------------------|-------|
| `dash`    | Runtime mapping + dev overlay           | `dash_x.exe`     | Supports IntelliSense |
| `itunes`  | Controls iTunes                         | `itunes.exe`     | Uses COM |
| `server`  | Hosts local file server                 | `server.exe`     | Simple HTTP server |
| `sp`      | Spotify history tracking                | `sp.exe`         | Stores play history |
| `wake`    | Tracks system wake events               | `wake.exe`       | Logs resume timestamps |

To differentiate component scripts from main scripts, the following notation is used:
- _x - this indicates an external component
- _c - this indicates a class for the component
- _t - this indicates a thread for the component

The main component will have:
sp.ixx
while the external component will have:
sp_x.ixx

The components talk to each other through pipes.

Modules that are shared are marked by adding a \hardlink tag.

## Adding a New Component

To add a new component to Auto Core, use the `core` directory as the destination.

### 1. Create Symbolic Link (Optional)
To ensure Visual Studio works correctly across directories, you may want to create a symbolic link to the project folder. In Command Prompt, run:

```
cd $(SolutionDir)
mklink /D .\.vs "C:\DJ\Programming\Project Files\Auto Core\"
```

### 2. Visual Studio Project Settings

Update the following project settings in Visual Studio:

- **Output Directory**  
  ```
  ..\..\bin
  ```

- **Intermediate Directory**  
  ```
  C:\DJ\Programming\Project Files\Auto Core\$(ProjectName)\
  ```

- **Linker → General → Additional Library Directories**  
  ```
  C:\DJ\My Folder\Auto Core\shared\auto_core_dll\
  ```

- **Linker → Input → Additional Dependencies**  
  ```
  auto_core_dll.lib;
  ```

### 3. Resource File
If your component uses a resource image or manifest, reference the shared resource file:

```
.\assets\resource.rc
```

This setup ensures your component builds correctly, links to the shared runtime library, and integrates seamlessly with the existing Auto Core architecture.

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
The `config` folder holds all runtime and environment settings for Auto Core, while the `dash` folder provides a developer-facing overlay to support editing, debugging, and IntelliSense integration in VS Code.

- `clock.ini` — Defines `end_of_day`, such as `00:59` vs. `24:59`.
- `itunes.ini` - Number of tabs to copy from iTunes.
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

## Shared Folder
- auto_core_dll - contains auto_core_dll.lib and the runtime .dll
- core_runtime - contains the shared header files
- pipes_x - contains the pipes file to be used by components if ipc is needed

## Tagging Runtime Functions
To make a function available for runtime configuration, tag it with \runtime.

After tagging, run dash_x.exe to update the function list. This ensures that the new function is recognized and available for use in runtime_map.ini.

Note: Runtime configuration must be enabled in config/runtime.ini for these functions to take effect.

## Spotify
The Spotify component enables for music integration with Spotify. The component maintains a database of the user's history because such data isn't immediately available by Spotify.

## Runtime Performance

The runtime behavior of Auto Core can be adjusted by modifying the configuration settings in `config\runtime.ini`. There are four available levels, each providing a tradeoff between observability and performance:

- **debug** – Enables full runtime debugging. Requires `runtime_enabled=true` and `runtime_debugger=true`.
- **buffer** – Enables buffered logging for runtime events. Requires `runtime_enabled=true` and `runtime_logger=true`.
- **silence** – Disables logging but keeps runtime mapping active. Requires `runtime_enabled=true` and `runtime_logger=false`.
- **disabled** – Disables runtime mapping entirely. Requires `runtime_enabled=false`. All mappings must be hardcoded.

### Performance Benchmarks (as of April 13, 2025):

| Runtime Mode | Average Time per Call |
|--------------|------------------------|
| `debug`      | 230–250 µs             |
| `buffer`     | 105–130 µs             |
| `silence`    | 75–95 µs               |
| `disabled`   | 0.20–0.35 µs           |

These benchmarks represent the overhead added by runtime configuration handling. Disabling runtime yields the highest performance and is recommended for production builds where configurability is not needed.

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

# Custom Firefox Configuration Guide

## Adjust New Tab Settings

1. Add the extension **New Tab Override**.
2. Go to the settings for **New Tab Override**.
3. Under the section **Option**, there is a dropdown box. Select the option for 'custom url'.
4. A section for **URL** will appear under **Option**.
   - Add the url for the local server. Example: `http://localhost:8585`
5. Under the section **Focus**, select the checkbox for 'Set focus to the web page instead of the address bar'.

## Configure Firefox Settings

1. In the address bar, go to `about:config`.
2. Modify the following preferences:
   - `toolkit.legacyUserProfileCustomizations.stylesheets` --> `true`
   - `browser.newtabpage.activity-stream.improvesearch.handoffToAwesomebar` --> `false`

## Retrieve Firefox Profile

1. In the address bar, go to `about:profiles`.
2. Retrieve the link for the root directory of the active profile. Example: `C:\Users\DJ\AppData\Roaming\Mozilla\Firefox\Profiles\[ProfileID]`

## Create Chrome Folder

1. Create a folder named 'chrome' in your Firefox profile.
2. Optional: Use a hard link for better organization. This step is recommended for users who prefer to keep their Firefox style settings in a separate location for easier management.
    - In an elevated console, use this command: `mklink /D Link Target`
    - Link - this is the path of your Firefox 'chrome' folder
    - Target - this is the location of your Firefox style settings

## Create CSS Files
1. Inside your style folder, create two files:
   - `userChrome.css`
   - `userContent.css`
2. Add the following to each file:
#### Contents of `userChrome.css`
    #nav-bar {
        visibility: collapse !important;
    }
    #alltabs-button {
        display: none !important;
    }
#### Contents of `userContent.css`
    #sidebarContainer {
        display: none !important;
    }
    #viewerContainer {
        left: 0 !important;
        right: 0 !important;
        top: 0 !important; 
    }
    .toolbar * {
        visibility: collapse !important;
    }
### Visual Studio Files
The `.vs` folder for any project should be set as a symbolic link that points to:
`C:\DJ\Programming\Project Files\Auto Core\.vs`

**Command Prompt:**
```sh
cd $(SolutionDir)
mklink /D .\.vs "C:\DJ\Programming\Project Files\Auto Core\.vs"
```

Set the intermediate directory:
C:\DJ\Programming\Project Files\Auto Core\$(ProjectName)\

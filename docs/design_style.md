## Directory for Project Files
### Visual Studio Files
The `.vs` folder for any project should be set as a symbolic link that points to:
`C:\DJ\Programming\Project Files\Auto Core\.vs`

**Command Prompt:**
```sh
cd $(SolutionDir)
mklink /D .\.vs "C:\DJ\Programming\Project Files\Auto Core\.vs"
```


### Intermediate Directory for Projects
To set the intermediate directory:
- Open the project properties in Visual Studio:
  - Go to `Project > Properties`
  - Navigate to `Configuration Properties > General > Intermediate Directory`
- Set the intermediate directory to:
  `C:\DJ\Programming\Project Files\Auto Core\$(ProjectName)\`

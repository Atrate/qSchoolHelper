# qSchoolHelper installer

Created with the help of: https://doc.qt.io/qtinstallerframework/ifw-tutorial.html

## Deployment:
- Compile qSH as per the main README.md
- Compress the compiled data to a .7z archive
- Place the compressed archive in ./installer/packages/qschoolhelper/data
- Run `..\..\bin\binarycreator.exe -c config\config.xml -p packages qSchoolHelperInstaller.exe`

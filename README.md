# Brainstorm Engine
## Installation
### Source
1. Download zip archive above
2. Extract
3. Navigate to extracted folder and open .sln file with Visual Studio
4. Click "Local Windows Debugger" toolbar button to run the example!
### Release
1. Navigate to the latest release
2. Download a zip archive
3. Extract it to your project libraries folder (it has "include" and "lib" folder)
4. Add include and lib folders in your project settings
5. Add Brainstorm.lib dependency in Linker > Input > Additional dependencies
6. Build your new app with Brainstorm Engine!

## Source Navigation
### Prepare
Make sure you've toggled "Show All Files" checkbox in Solution Explorer like on image below:
!["Show All Files" Toggle example](https://i.ibb.co/WKpZXCH/result-png.png)
### Project Structure
- Framework/
	- libraries/ - Libraries framework use
	- src/ - Source code of the framework
- Test/
	- libraries/ - Libraries generated by the framework
	- src/ - Source code of the example project
### Useful Info
If you've changed something in Framework solution, just right click on it and click "Build", if build was successful, you've done!
After that you can just continue work in Test or your own solution as before.

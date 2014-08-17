Building EA WebKit
---------------
EA WebKit supports Win32, Win64, Playstation 3, and Xbox 360 platforms.  
EA WebKit requires that you have proper development tools installed.

1. Install developer tools
2. Open \projects\<Visual Studio Version>\EAWebKit\<EA WebKit Version No>\EAWebKit.sln
3. Select appropriate platform\project from toolbar.
4. Execute Build.

Developer Requirements
----------------------
*General*
EA WebKit requires Visual Studio 2005 or 2008 to build.  
Visual Studio can be obtained from Microsoft Corporation from http://www.microsoft.com/visualstudio/en-us/default.mspx.

For Visual Studio 2005, the following service pack will be required:
* Service Pack 1 for Visual Studio 2005

It is expected that Visual Studio will be installed under C:\Program Files\Microsoft Visual Studio <version>.  
For VS 2005, that is C:\Program Files\Microsoft Visual Studio 8, and for VS 2008, C:\Program Files\Microsoft Visual Studio 9.

Windows SDK version 6.0.6001.18000.367 is also required and can be obtained at 
http://www.microsoft.com/downloads/details.aspx?FamilyId=F26B1AA4-741A-433A-9BE5-FA919850BDBF.  
The SDK should be installed at C:\Program Files\Microsoft SDKs\Windows\v6.0A .

*Sony Playstation 3*
If building on Sony Playstation 3, installation of Playstation 3 SDK 330.001 is required.  
Playstation 3 SDK can be acquired by contacting Sony at http://www.scedev.net/.  
The Sony SN-VSI 1.8.2 tool will also be required to build.

SCE_PS3_ROOT should be set up as an environment variable and point to the local \cell directory.

*Microsoft Xbox 360*
If building on Microsoft Xbox 360, installation of XDK 2.0.11164.3 is required.  
XDK can be acquired by contacting Microsoft at http://www.xbox.com/en-US/dev/tools.htm.  

Xbox360InstallDir should be set up as an environment variable and point to the local Microsoft Xbox 360 SDK directory.

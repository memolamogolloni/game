ofxWinTouch
=======
Compatible with openFrameworks 0.8.0

Note - this version (ofxWinTouch) introduces breaking changes from the original ofxWin7. I changed the order of the arguments in the touch callbacks to match existing oF implementations, and changed touchMove to touchMoved.

Simple helper classes to setup native Windows multitouch in openFrameworks. Supports both Windows 7/8 touch events as well as a gestures. 

Based on code from the OF forum here: http://forum.openframeworks.cc/index.php/topic,3120.new.html#new

Setting Up
=======
* You'll need to add the following preprocessor defs to your app to compile in Windows 7+ touch support:
  * ```_WIN32_WINNT=0x0601```
  * ```POCO_NO_UNWINDOWS```

Windows Surface / High DPI
=======
Some devices (like the Windows Surface Pro) have a default "High DPI" setting to make text more readable on the screen. 
On the Surface Pro, the default settings is 150% DPI - which means a display resolution of 1920x1080 will report a screen size of 1280x720 to openFrameworks, but still report touches on a 1920x1080 grid.
Thankfully, there's an easy fix:
 * Open Project Properties
 * Navigate to Configuration Properties > Manifest Tool > Input and Output
 * Set 'Enable DPI Awareness' to Yes

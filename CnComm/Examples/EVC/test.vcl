<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: test - Win32 (WCE emulator) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "c:\temp\RSP2B.tmp" with contents
[
/nologo /W3 /D "_i386_" /D UNDER_CE=400 /D _WIN32_WCE=400 /D "WCE_PLATFORM_STANDARDSDK" /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "NDEBUG" /D "_WIN32_WCE_CEPC" /D "_AFXDLL" /Fp"emulatorRel/test.pch" /Yu"stdafx.h" /Fo"emulatorRel/" /Gs8192 /GF /O2 /c 
"E:\CnComm\Examples\EVC\testDlg.cpp"
]
Creating command line "cl.exe @c:\temp\RSP2B.tmp" 
Creating temporary file "c:\temp\RSP2C.tmp" with contents
[
/nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"wWinMainCRTStartup" /incremental:no /pdb:"emulatorRel/test.pdb" /out:"emulatorRel/test.exe" /subsystem:windowsce,4.00 /MACHINE:IX86 
.\emulatorRel\StdAfx.obj
.\emulatorRel\test.obj
.\emulatorRel\testDlg.obj
.\emulatorRel\test.res
]
Creating command line "link.exe @c:\temp\RSP2C.tmp"
<h3>Output Window</h3>
Compiling...
testDlg.cpp
Linking...




<h3>Results</h3>
test.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>

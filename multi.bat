@ECHO OFF
CL /I".." /D STRICT /W4 /c multi.cpp
LINK user32.lib kernel32.lib multi.obj /NOENTRY /DLL /DEF:multi.def /OPT:REF /OPT:ICF /OUT:multi.rpi

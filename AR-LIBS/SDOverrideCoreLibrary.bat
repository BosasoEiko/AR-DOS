@echo off
cd %USERPROFILE%\AppData\Local\Arduino15\libraries\SD\
del *.*
cd ..
rmdir SD
pause
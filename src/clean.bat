@echo off
REM Run this to erase the unnecessary intermediate files and folders.
attrib -s -h *.* /s
rmdir obj /s /q
del *.ncb /s
del *.aps /s
del *.pch /s
del *.suo /s
del Thumbs.db /s
pause
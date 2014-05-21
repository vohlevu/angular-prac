@echo off
set DEV_PATH=.\build\Release\controller.node> nul
set RELEASE_PATH=bin> nul
echo Release %DEV_PATH% to %RELEASE_PATH%
xcopy /f /y %DEV_PATH% %RELEASE_PATH% > nul
@echo on

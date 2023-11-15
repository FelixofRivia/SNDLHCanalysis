@echo off
setlocal enabledelayedexpansion
REM Check if four arguments are provided
if "%4"=="" (
    echo Please provide four numbers as arguments.
) else (

    set /a "glass=(%1 + 2)/5 * 5"
    set /a "bottom=((%2-%1) + 2)/5 * 5"
    set /a "base=((%3-%2) + 2)/5 * 5"
    set /a "top=((%4-%3) + 2)/5 * 5"


    echo Glass: !glass!
    echo Bottom: !bottom!
    echo Base: !base!
    echo Top: !top!
)
endlocal
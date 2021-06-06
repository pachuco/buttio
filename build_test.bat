@echo off
call getcomp.bat rosbe


set opts=-std=c99 -mconsole -Wall -Wextra -g
set opts=-std=c99 -mconsole -Wall -Wextra -Os -s
set opts=%opts% -Wl,--enable-stdcall-fixup -static-libgcc
set linkinc=-lbuttio -L.\bin\

set files=.\src\test.c
set errlog=.\test_err.log

del .\bin\test.exe
gcc -o .\bin\test.exe %files% %opts% %linkinc% 2> %errlog%
call :checkerr
exit /B 0



:checkerr
IF %ERRORLEVEL% NEQ 0 (
    echo oops!
    notepad %errlog%
    goto :end
)
for %%R in (%errlog%) do if %%~zR lss 1 del %errlog%
:end
exit /B 0
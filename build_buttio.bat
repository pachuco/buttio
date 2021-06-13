@echo off

call getcomp rosbe

::set DBG=-DDBG
set opts=-std=c99 -mconsole -Wall -Wextra -Os -s ^
  -Wl,--entry,_DriverEntry@8,-shared,--subsystem,native ^
  -nostartfiles -nodefaultlibs -nostdlib ^
  -mstackrealign -mpreferred-stack-boundary=3 -fno-set-stack-executable ^
  -Wl,--enable-auto-image-base,--disable-auto-import ^
  -Wl,--disable-stdcall-fixup,--exclude-all-symbols ^
  -Wl,--image-base,0x10000,-section-alignment=0x1000 ^
  -DCODEISDRIVER %DBG%


set linkinc=-I=/i686-w64-mingw32/include/ddk

set linkinc=%linkinc% -lntoskrnl -lhal
set compiles=.\src\buttio_krn.c .\src\buttio_common.c
set errlog=.\buttio_krn_err.log

del .\bin\buttio.sys
gcc -o .\bin\buttio.sys %compiles% %opts% %linkinc% 2> %errlog%
call :checkerr




set opts=-std=c99 -Wall -Wextra -Os -s

set linkinc=
set errlog=.\buttio_usr_err.log

del bin\*.a
gcc -o .\bin\buttio_usr.o -c .\src\buttio_usr.c %opts%       2> %errlog%
call :checkerr
gcc -o .\bin\buttio_common.o -c .\src\buttio_common.c %opts% 2> %errlog%
call :checkerr
gcc -o .\bin\util.o -c .\src\util.c %opts%                   2> %errlog%
call :checkerr
ar r bin\libbuttio.a bin\*.o
del bin\*.o








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
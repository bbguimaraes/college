masm51\masm\masm.exe c.asm ;
IF ERRORLEVEL 1 GOTO ERROR
masm51\utilit~1\link.exe c.obj ;
IF ERRORLEVEL 1 GOTO ERROR
c.exe
:ERROR

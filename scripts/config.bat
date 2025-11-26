set projectDir=%~1
set outDir=%~2

@echo off

echo "Copiando archivos..."

copy "%projectDir%\lib\SFML-3.0.2\bin" "%outDir%" /y 
xcopy "%projectDir%\res" "%outDir%\res" /y /i /e /s
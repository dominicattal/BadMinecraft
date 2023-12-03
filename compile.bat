@echo off
setlocal enableextensions enabledelayedexpansion
set Cfiles=
set Cfiles=%Cfiles% C:\Users\attal\Documents\Projects\OpenGL\BadMinecraft\lib\glad\src\glad_gl.c
set Cfiles=%Cfiles% C:\Users\attal\Documents\Projects\OpenGL\BadMinecraft\lib\stb\src\stb_image.c
for /R .\src %%f in (*) do (
    if %%~xf==.c set "Cfiles=!Cfiles! %%f"
)
gcc -I./lib/glad/include -I./lib/glfw/include -I./lib/stb/include -L./lib/glfw/link %Cfiles% -lglfw3dll -o prog
prog
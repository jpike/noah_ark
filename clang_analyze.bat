WHERE cl.exe
IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

clang-cl.exe --version

IF %ERRORLEVEL% NEQ 0 SET PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\bin;%PATH%

ECHO %PATH%

REM DEFINE COMPILER OPTIONS.
SET COMMON_COMPILER_OPTIONS=/EHsc /WX /W4 /TP /std:c++latest /DSFML_STATIC /D_DEBUG
SET DEBUG_COMPILER_OPTIONS=%COMMON_COMPILER_OPTIONS% /Z7 /Od
REM Not allowed with -fsanitize=address /MTd

REM DEFINE FILES TO COMPILE/LINK.
REM These paths must be relative to the build directory in which the actual compilation command is executed.
SET COMPILATION_FILE="..\noah_ark_library.project"
SET MAIN_CODE_DIR="..\code\library"
SET SFML_DIR="..\ThirdParty\SFML"
SET SFML_CODE_DIR="%SFML_DIR%\include"
SET SFML_EXTERNAL_LIB_DIR="%SFML_DIR%\extlibs\libs-msvc\x64"

REM CREATE THE COMMAND LINE OPTIONS FOR THE FILES TO COMPILE/LINK.
SET INCLUDE_DIRS=/I %MAIN_CODE_DIR% /I %SFML_CODE_DIR%
SET PROJECT_FILES_DIRS_AND_LIBS=%COMPILATION_FILE% %INCLUDE_DIRS%

REM MOVE INTO THE BUILD DIRECTORY.
SET build_directory=build\%build_mode%
IF NOT EXIST "%build_directory%" MKDIR "%build_directory%"
PUSHD "%build_directory%"

    REM BUILD THE PROGRAM BASED ON THE BUILD MODE.
    REM  -target=x86-64-pc-windows-msvc19.24.28319
    clang-cl.exe -m64 -fsanitize=cfi -flto -fuse-ld=lld -v %DEBUG_COMPILER_OPTIONS% %PROJECT_FILES_DIRS_AND_LIBS%

    SET COMPILATION_FILE="..\code\game\main.cpp"
    SET SFML_LIB_DIR="%SFML_DIR%\build\lib\Debug"
    SET SFML_LIBRARIES=sfml-audio-s-d.lib sfml-graphics-s-d.lib sfml-main-d.lib sfml-system-s-d.lib sfml-window-s-d.lib
    SET OTHER_THIRD_PARTY_LIBS=opengl32.lib freetype.lib winmm.lib gdi32.lib openal32.lib flac.lib vorbisenc.lib vorbisfile.lib vorbis.lib ogg.lib user32.lib advapi32.lib
    SET LIBRARIES=noah_ark_library.lib %SFML_LIBRARIES% %OTHER_THIRD_PARTY_LIBS%
    SET PROJECT_FILES_DIRS_AND_LIBS=%COMPILATION_FILE% %INCLUDE_DIRS% /link %LIBRARIES% /LIBPATH:%SFML_LIB_DIR% /LIBPATH:%SFML_EXTERNAL_LIB_DIR%
    clang-cl.exe -m64 -fsanitize=cfi -flto -fuse-ld=lld -v %DEBUG_COMPILER_OPTIONS% %PROJECT_FILES_DIRS_AND_LIBS%

POPD

ECHO Done (analyze)

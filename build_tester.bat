@ECHO OFF

REM INITIALIZE THE COMPILER ENVIRONMENT.
WHERE cl.exe
IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
REM CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
WHERE cl.exe

SET TOOL_DIRECTORY_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.25.28610\bin\Hostx64\x64"
SET COMPILER_PATH="%TOOL_DIRECTORY_PATH%\cl.exe"
SET LIB_TOOL_PATH="%TOOL_DIRECTORY_PATH%\lib.exe"

REM READ THE BUILD MODE COMMAND LINE ARGUMENT.
REM Either "debug" or "release" (no quotes).
REM If not specified, will default to debug.
IF EXIST "%1" (
    SET build_mode=%1
) ELSE (
    SET build_mode=debug
)

REM DEFINE COMPILER OPTIONS.
SET COMMON_COMPILER_OPTIONS=/EHsc /WX /W4 /TP /std:c++latest /Fo:noah_ark_tester /DSFML_STATIC
SET DEBUG_COMPILER_OPTIONS=%COMMON_COMPILER_OPTIONS% /Z7 /Od /MTd
SET RELEASE_COMPILER_OPTIONS=%COMMON_COMPILER_OPTIONS% /O2 /MT

REM DEFINE FILES TO COMPILE/LINK.
REM These paths must be relative to the build directory in which the actual compilation command is executed.
SET COMPILATION_FILE="..\..\code\testing\main.cpp"
SET MAIN_CODE_DIR="..\..\code\library"
SET CATCH_DIR="..\..\..\ThirdParty\catch"
SET SFML_DIR="..\..\..\ThirdParty\SFML\SFML-2.5.1"
SET SFML_CODE_DIR="%SFML_DIR%\include"
SET SFML_EXTERNAL_LIB_DIR="%SFML_DIR%\extlibs\libs-msvc\x64"
REM Exact SFML libraries will vary depending on build mode.
IF "%build_mode%"=="release" (
    SET SFML_LIB_DIR="%SFML_DIR%\build\lib\Release"
    SET SFML_LIBRARIES=sfml-audio-s.lib sfml-graphics-s.lib sfml-main.lib sfml-system-s.lib sfml-window-s.lib
) ELSE (
    SET SFML_LIB_DIR="%SFML_DIR%\build\lib\Debug"
    SET SFML_LIBRARIES=sfml-audio-s-d.lib sfml-graphics-s-d.lib sfml-main-d.lib sfml-system-s-d.lib sfml-window-s-d.lib
)
SET OTHER_THIRD_PARTY_LIBS=opengl32.lib freetype.lib winmm.lib gdi32.lib openal32.lib flac.lib vorbisenc.lib vorbisfile.lib vorbis.lib ogg.lib user32.lib advapi32.lib
SET LIBRARIES=noah_ark_library.lib %SFML_LIBRARIES% %OTHER_THIRD_PARTY_LIBS%

REM CREATE THE COMMAND LINE OPTIONS FOR THE FILES TO COMPILE/LINK.
SET INCLUDE_DIRS=/I %MAIN_CODE_DIR% /I %SFML_CODE_DIR% /I %CATCH_DIR%
SET PROJECT_FILES_DIRS_AND_LIBS=%COMPILATION_FILE% %INCLUDE_DIRS% /link %LIBRARIES% /LIBPATH:%SFML_LIB_DIR% /LIBPATH:%SFML_EXTERNAL_LIB_DIR%

REM MOVE INTO THE BUILD DIRECTORY.
SET build_directory=build\%build_mode%
IF NOT EXIST "%build_directory%" MKDIR "%build_directory%"
PUSHD "%build_directory%"

    REM BUILD THE PROGRAM BASED ON THE BUILD MODE.
    IF "%build_mode%"=="release" (
        "%COMPILER_PATH%" %RELEASE_COMPILER_OPTIONS% %PROJECT_FILES_DIRS_AND_LIBS%
    ) ELSE (
        "%COMPILER_PATH%" %DEBUG_COMPILER_OPTIONS% %PROJECT_FILES_DIRS_AND_LIBS%
    )

POPD

ECHO Done (tester)

@ECHO ON

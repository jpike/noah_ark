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
SET COMMON_COMPILER_OPTIONS=/EHsc /WX /W4 /TP /std:c++latest /DSFML_STATIC
SET DEBUG_COMPILER_OPTIONS=%COMMON_COMPILER_OPTIONS% /Z7 /Od /MTd
SET RELEASE_COMPILER_OPTIONS=%COMMON_COMPILER_OPTIONS% /O2 /MT

REM DEFINE FILES TO COMPILE/LINK.
REM These paths must be relative to the build directory in which the actual compilation command is executed.
SET COMPILATION_FILE="..\..\noah_ark_library.project"
SET MAIN_CODE_DIR="..\..\code\library"
SET SFML_CODE_DIR="..\..\ThirdParty\SFML\include"
SET OPEN_AL_HEADER_DIR="..\..\ThirdParty\SFML\extlibs\headers\AL"

REM CREATE THE COMMAND LINE OPTIONS FOR THE FILES TO COMPILE/LINK.
SET INCLUDE_DIRS=/I %MAIN_CODE_DIR% /I %SFML_CODE_DIR% /I %OPEN_AL_HEADER_DIR%
SET PROJECT_FILES_DIRS_AND_LIBS=%COMPILATION_FILE% %INCLUDE_DIRS%

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
    
    "%LIB_TOOL_PATH%" "noah_ark_library.obj"

POPD

ECHO Done (library)

@ECHO ON

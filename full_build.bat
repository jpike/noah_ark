@ECHO OFF

REM READ THE BUILD MODE COMMAND LINE ARGUMENT.
REM Either "debug" or "release" (no quotes).
REM If not specified, will default to debug.
IF EXIST "%1" (
    SET build_mode=%1
) ELSE (
    SET build_mode=debug
)

WHERE cl.exe
REM IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
WHERE cl.exe

CALL build_library.bat %build_mode%
CALL build_asset_packer.bat %build_mode%
CALL build_tester.bat %build_mode%
CALL build_game.bat %build_mode%
CALL copy_dlls.bat %build_mode%
CALL copy_resources.bat %build_mode%

SET build_directory=build\%build_mode%
PUSHD "%build_directory%"
    noah_ark_tester.exe
    asset_packer.exe
POPD

@ECHO OFF

ECHO Done (full build)

@ECHO ON

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
REM IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
WHERE cl.exe

REM CALL build_font_metrics.bat %build_mode%
IF %ERRORLEVEL% EQU 0 (
    CALL build_library.bat %build_mode%
)
IF %ERRORLEVEL% EQU 0 (
    CALL build_asset_packer.bat %build_mode%
)
REM TODO RE-ENABLE AFTER FIXING IF %ERRORLEVEL% EQU 0 (
REM    CALL build_tester.bat %build_mode%
REM )
IF %ERRORLEVEL% EQU 0 (
    CALL build_game.bat %build_mode%
)
IF %ERRORLEVEL% EQU 0 (
    CALL copy_dlls.bat %build_mode%
)
IF %ERRORLEVEL% EQU 0 (
    CALL copy_resources.bat %build_mode%
)

IF %ERRORLEVEL% EQU 0 (
    SET build_directory=build\%build_mode%
    PUSHD "%build_directory%"
        IF %ERRORLEVEL% EQU 0 (
            noah_ark_tester.exe
        )
        IF %ERRORLEVEL% EQU 0 (
            asset_packer.exe
        )
    POPD
)

@ECHO OFF

ECHO Done (full build) (exit code = %ERRORLEVEL%)

@ECHO ON

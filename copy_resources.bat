@ECHO OFF

REM READ THE BUILD MODE COMMAND LINE ARGUMENT.
REM Either "debug" or "release" (no quotes).
REM If not specified, will default to debug.
IF NOT "%1" == "" (
    SET build_mode=%1
) ELSE (
    SET build_mode=debug
)

REM ENSURE THE APPROPRIATE BUILD DIRECTORY EXISTS.
SET build_directory=build\%build_mode%
IF NOT EXIST "%build_directory%" MKDIR "%build_directory%"

REM COPY OVER RAW RESOURCES.
XCOPY /Y /S "assets\res\*" "%build_directory%\res\"

ECHO Done (copy resources)

@ECHO ON

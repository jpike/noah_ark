@ECHO OFF

REM READ THE BUILD MODE COMMAND LINE ARGUMENT.
REM Either "debug" or "release" (no quotes).
REM If not specified, will default to debug.
IF EXIST "%1" (
    SET build_mode=%1
) ELSE (
    SET build_mode=debug
)

REM ENSURE THE APPROPRIATE BUILD DIRECTORY EXISTS.
SET build_directory=build\%build_mode%
IF NOT EXIST "%build_directory%" MKDIR "%build_directory%"

REM COPY OVER DLLs REQUIRED FOR SFML AUDIO USAGE.
SET SFML_DIR="ThirdParty\SFML"
SET SFML_EXTERNAL_LIB_DIR="%SFML_DIR%\extlibs"
XCOPY /Y "%SFML_EXTERNAL_LIB_DIR%\openal32.dll" "%build_directory%"

ECHO Done (copy DLLs)

@ECHO ON

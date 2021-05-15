@ECHO OFF

REM READ THE VERSION NUMBER COMMAND LINE ARGUMENT.
SET release_version=%1

WHERE cl.exe
IF %ERRORLEVEL% NEQ 0 CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
WHERE cl.exe

IF %ERRORLEVEL% EQU 0 (
    CALL full_build.bat release
)

IF %ERRORLEVEL% EQU 0 (
    MKDIR build\release\ThirdPartyLicenses
    COPY ThirdParty\AdditionalSound_Licenses.txt build\release\ThirdPartyLicenses\AdditionalSound_Licenses.txt
    COPY ThirdParty\catch\Catch_License.txt build\release\ThirdPartyLicenses\Catch_License.txt 
    COPY ThirdParty\SFML\SFML_License.txt build\release\ThirdPartyLicenses\SFML_License.txt
    COPY ThirdParty\GSL\LICENSE build\release\ThirdPartyLicenses\GSL_License.txt
    COPY ThirdParty\GSL\ThirdPartyNotices.txt build\release\ThirdPartyLicenses\GSL_ThirdPartyNotices.txt

    COPY CREDITS.txt build\release\CREDITS.txt
    COPY LICENSE.txt build\release\LICENSE.txt
    COPY documentation\README.txt build\release\README.txt
)

IF %ERRORLEVEL% EQU 0 (
    PUSHD build\release
    REM a - Add files to archive.
    REM t - Archive type.
    P:\file_compression\7-Zip\7z.exe a -tzip ..\..\releases\noah_ark_game_%release_version%.zip noah_ark_game.exe openal32.dll IntroAssets.package MainAssets.package CREDITS.txt LICENSE.txt ThirdPartyLicenses

    POPD
)

@ECHO OFF

ECHO Done (release build %release_version%) (exit code = %ERRORLEVEL%)

@ECHO ON

setlocal enabledelayedexpansion

set QTLIBSPATH=
set QTMINGWPATH=
set DULLAHANFFMPEGMINGWBIN=
set MINGWPATH=
set MINGWSUBPATH=
set QMAKESPEC=
set MINGWMAKEBINARY=
set DULLAHANFFMPEGDIRECTORY=
set SLEEPBINARY=C:\msys64\usr\bin\sleep.exe
set CPBINARY=C:\msys64\usr\bin\cp.exe
set QTWINDEPLOYBINARY=

set RAIDENVIDEORIPPERVERISON=1.0.1.0

set configure_flags=
set target_arch=NULL
set dullahan_ffmpeg_build=true

echo off

for %%x in (%*) do (
    echo %%~x
    if %%~x==--configure (
        echo "Full build enabled..."
        set configure_flags=%configure_flags%--configure 
    ) else if %%x==--clean (
        echo "Clean..."
        set configure_flags=%configure_flags%--clean
    ) else if %%~x==--x86 (
        set QTLIBSPATH=C:\Users\Demensdeum\Documents\Sources\3rdParty\qt\build\qtbase\lib
        set QTMINGWPATH=C:\Users\Demensdeum\Documents\Sources\3rdParty\qt\build\qtbase\bin
        set target_arch=--x86
        set MINGWSUBPATH=C:\Users\Demensdeum\Documents\Sources\3rdParty\winlibs-ming32\bin
        set DULLAHANFFMPEGMINGWBIN=C:\msys64\mingw32.exe
        set MINGWPATH=C:\Users\Demensdeum\Documents\Sources\3rdParty\winlibs-ming32\bin
        set MINGWMAKEBINARY=C:\Users\Demensdeum\Documents\Sources\3rdParty\winlibs-ming32\bin\mingw32-make.exe
        set QMAKESPEC=win32-g++
        set DULLAHANFFMPEGDIRECTORY=C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg
        set DEPENDENCYLIBSPATH=C:\msys64\mingw32\bin
        set QTWINDEPLOYBINARY=C:\Users\Demensdeum\Documents\Sources\3rdParty\qt\build\qtbase\bin\windeployqt.exe
    ) else if %%~x==--x86_64 (
        set QTLIBSPATH=C:/Qt/6.6.0/mingw_64/lib/        
        set QTMINGWPATH=C:\Qt\6.6.0\mingw_64\bin
        set target_arch=--x86_64
        set MINGWSUBPATH=C:\msys64\mingw64\bin
        set DULLAHANFFMPEGMINGWBIN=C:\msys64\mingw64.exe
        set MINGWPATH=C:\msys64\usr\bin
        set MINGWMAKEBINARY=C:\Qt\Tools\mingw1120_64\bin\mingw32-make.exe        
        set QMAKESPEC=win32-g++
        set DULLAHANFFMPEGDIRECTORY=C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg
        set DEPENDENCYLIBSPATH=C:\msys64\mingw64\bin
        set QTWINDEPLOYBINARY=C:\Qt\6.6.0\mingw_64\bin\windeployqt.exe
    ) else if %%~x==--skip-dullahan-ffmpeg (
        echo Skipping Dullahan-FFmpeg build
        set dullahan_ffmpeg_build=false
    ) else (
        echo Unknown argument %%~x
        exit 1
    )
)

echo Target arch: %target_arch%

if %target_arch%==NULL (
    echo "No arch selected! Use (--x86 or --x86_64 flags)"
    exit 2
)

set PATH=%MINGWPATH%;%QTMINGWPATH%;%MINGWSUBPATH%;%QTLIBSPATH%;%PATH%

echo on

if %dullahan_ffmpeg_build%==true (
echo "Building Dullahan-FFmpeg..."
del C:\msys64\tmp\Dullahan-FFmpeg-Build-Complete
cd C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg

%DULLAHANFFMPEGMINGWBIN% C:\msys64\usr\bin\bash.exe -c "./build.sh --release --install-libs --run-examples %configure_flags% %target_arch% ; exec bash"

:loop
echo off
if not exist "C:\msys64\tmp\Dullahan-FFmpeg-Build-Complete" (
    echo on
    echo "Waiting for FFmpeg-Dullahan build script to complete..."
    echo off
    %SLEEPBINARY% 10
    goto :loop
)
)

echo on

echo "Building Raiden Video Ripper..."
cd C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper
set buildDirectory=C:\RaidenVideoRipperReleaseVersion
rmdir /S /Q %buildDirectory%
mkdir %buildDirectory%
cd %buildDirectory%

echo %PATH%

%QTMINGWPATH%\qmake C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\RaidenVideoRipper.pro -spec %QMAKESPEC%

%MINGWMAKEBINARY% -j16

@echo off
if %ERRORLEVEL% neq 0 (
    @echo on
    echo RVR build failed, return code: %ERRORLEVEL%.
    @echo off
    exit %ERRORLEVEL%
)

cd release

%QTWINDEPLOYBINARY% RaidenVideoRipper.exe

%CPBINARY% %DULLAHANFFMPEGDIRECTORY%\libavfilter\avfilter-9.dll .
%CPBINARY% %DULLAHANFFMPEGDIRECTORY%\libavcodec\avcodec-60.dll .
%CPBINARY% %DULLAHANFFMPEGDIRECTORY%\libavdevice\avdevice-60.dll .
%CPBINARY% %DULLAHANFFMPEGDIRECTORY%\libavformat\avformat-60.dll .
%CPBINARY% %DULLAHANFFMPEGDIRECTORY%\libavutil\avutil-58.dll .
%CPBINARY% %DULLAHANFFMPEGDIRECTORY%\libpostproc\postproc-57.dll .
%CPBINARY% %DULLAHANFFMPEGDIRECTORY%\libswresample\swresample-4.dll .
%CPBINARY% %DULLAHANFFMPEGDIRECTORY%\libswscale\swscale-7.dll .
%CPBINARY% %DULLAHANFFMPEGDIRECTORY%\dullahan_ffmpeg.dll .

%CPBINARY% %DEPENDENCYLIBSPATH%\libiconv-2.dll .
%CPBINARY% %DEPENDENCYLIBSPATH%\liblzma-5.dll .
%CPBINARY% %DEPENDENCYLIBSPATH%\libvorbis-0.dll .
%CPBINARY% %DEPENDENCYLIBSPATH%\libvorbisenc-2.dll .
%CPBINARY% %DEPENDENCYLIBSPATH%\libx264-164.dll .
%CPBINARY% %DEPENDENCYLIBSPATH%\libvpx-1.dll .
%CPBINARY% %DEPENDENCYLIBSPATH%\libbz2-1.dll .
%CPBINARY% %DEPENDENCYLIBSPATH%\zlib1.dll .
%CPBINARY% %DEPENDENCYLIBSPATH%\libogg-0.dll .
%CPBINARY% %DEPENDENCYLIBSPATH%\SDL2.dll .
%CPBINARY% %QTMINGWPATH%\libmcfgthread-1.dll .

start RaidenVideoRipper.exe

cd ..
%SLEEPBINARY% 2
del release /F /Q
taskkill /f /t /im RaidenVideoRipper.exe
cd release
%CPBINARY% C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\COPYING.LGPLv3 .
%CPBINARY% C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\COPYING.GPLv2 .
%CPBINARY% C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\COPYING.GPLv3 .
%CPBINARY% C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\COPYING.LGPLv2.1 .
%CPBINARY% C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\LICENSE .
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" "C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\installer\config.iss"
cd ..
mkdir zip
cd zip
%CPBINARY% -r ../release .
rename release RaidenVideoRipper
tar acvf RaidenVideoRipper-%RAIDENVIDEORIPPERVERISON%.zip RaidenVideoRipper

cd ..
explorer installer
explorer zip

endlocal
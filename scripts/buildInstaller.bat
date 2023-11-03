set MINGWPATH=C:\msys64\usr\bin
set MINGWSUBPATH=C:\msys64\mingw64\bin
set QTMINGWPATH=C:\Qt\6.6.0\mingw_64\bin
set PATH=%MINGWPATH%;%QTMINGWPATH%;%MINGWSUBPATH%;%PATH%
set MINGWPATH=
set QTMINGWPATH=
set MINGWSUBPATH=

set RAIDENVIDEORIPPERVERISON=1.0.0.0

set configure_flags=

@echo off
if "%~1"=="--full-build" (
    echo "Full build enabled..."
    set configure_flags=--configure --clean
)
@echo on

echo "Building Dullahan-FFmpeg..."
del C:\msys64\tmp\Dullahan-FFmpeg-Build-Complete
cd C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg
C:\msys64\mingw64.exe C:\msys64\usr\bin\bash.exe build.sh --release --install-libs --run-examples %configure_flags%

:loop
@echo off
if not exist "C:\msys64\tmp\Dullahan-FFmpeg-Build-Complete" (
    @echo on
    echo "Waiting for FFmpeg-Dullahan build script to complete..."
    @echo off
    sleep 10
    goto :loop
)

echo "Building Raiden Video Ripper..."
cd C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper
set buildDirectory=C:\RaidenVideoRipperReleaseVersion
rmdir /s /q %buildDirectory%
mkdir %buildDirectory%
cd %buildDirectory%
C:\Qt\6.6.0\mingw_64\bin\qmake C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\RaidenVideoRipper.pro
make
cd release
C:\Qt\6.6.0\mingw_64\bin\windeployqt.exe RaidenVideoRipper.exe
cp C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg\libavfilter/avfilter-9.dll .
cp C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg\libavcodec/avcodec-60.dll .
cp C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg\libavdevice/avdevice-60.dll .
cp C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg\libavformat/avformat-60.dll .
cp C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg\libavutil/avutil-58.dll .
cp C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg\libpostproc/postproc-57.dll .
cp C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg\libswresample/swresample-4.dll .
cp C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg\libswscale/swscale-7.dll .
cp C:\msys64\home\Demensdeum\Sources\Dullahan-FFmpeg\dullahan_ffmpeg.dll .
cp C:\msys64\ucrt64\bin\libiconv-2.dll .
cp C:\msys64\mingw64\bin\liblzma-5.dll .
cp C:\msys64\mingw64\bin\libvorbis-0.dll .
cp C:\msys64\mingw64\bin\libvorbisenc-2.dll .
cp C:\msys64\mingw64\bin\SDL2.dll .
cp C:\msys64\mingw64\bin\libx256.dll .
cp C:\msys64\mingw64\bin\libx264-164.dll .
cp C:\msys64\mingw64\bin\libvpx-1.dll .
cp C:\msys64\mingw64\bin\libbz2-1.dll .
cp C:\msys64\mingw64\bin\zlib1.dll .
cp C:\msys64\mingw64\bin\libogg-0.dll .
cp C:\msys64\mingw64\bin\libx265.dll .
start RaidenVideoRipper.exe
cd ..
sleep 2
del release /F /Q
taskkill /f /t /im RaidenVideoRipper.exe
cd release
cp C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\COPYING.LGPLv3 .
cp C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\COPYING.GPLv2 .
cp C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\COPYING.GPLv3 .
cp C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\COPYING.LGPLv2.1 .
cp C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\LICENSE .
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" "C:\Users\Demensdeum\Documents\Sources\RaidenVideoRipper\installer\config.iss"
cd ..
mkdir zip
cd zip
cp -r ../release .
rename release RaidenVideoRipper
tar acvf RaidenVideoRipper-%RAIDENVIDEORIPPERVERISON%.zip RaidenVideoRipper
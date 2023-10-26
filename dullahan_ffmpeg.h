#pragma once

#include <stdbool.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport)int dullahan_ffmpeg_main(int argc, char **argv);
__declspec(dllexport)void dullahan_ffmpeg_cancel();
__declspec(dllexport)bool dullahan_ffmpeg_conversion_aborted();
__declspec(dllexport)uint64_t dullahan_ffmpeg_conversion_pts();
#ifdef __cplusplus
}
#endif
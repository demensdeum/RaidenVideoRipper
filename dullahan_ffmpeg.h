#pragma once

#include <stdbool.h>
#include <inttypes.h>

#ifdef _WIN32
#define DULLAHAN_EXPORT __declspec(dllexport)
#else
#define DULLAHAN_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

DULLAHAN_EXPORT int dullahan_ffmpeg_main(int argc, char **argv);
DULLAHAN_EXPORT void dullahan_ffmpeg_cancel();
DULLAHAN_EXPORT bool dullahan_ffmpeg_conversion_aborted();
DULLAHAN_EXPORT int64_t dullahan_ffmpeg_conversion_pts();

#ifdef __cplusplus
}
#endif

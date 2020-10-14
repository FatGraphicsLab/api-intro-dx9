#pragma once

#if defined(FAT_DEBUG_BUILD)
#  define FAT_DEBUG_BUILD   1
#  define FAT_RELEASE_BUILD 0
#elif defined(FAT_RELEASE_BUILD)
#  define FAT_DEBUG_BUILD   0
#  define FAT_RELEASE_BUILD 1
#endif

#if FAT_DEBUG_BUILD
// https://github.com/kasicass/blog/blob/master/3dapi-d3d9/2020_08_29_d3d_debug_info.md
#  ifndef D3D_DEBUG_INFO
#  define D3D_DEBUG_INFO
#  endif

// Enable run-time memory check for debug builds
// https://github.com/kasicass/blog/blob/master/fat3d/2020_09_11_memory_leak_detection.md
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#endif

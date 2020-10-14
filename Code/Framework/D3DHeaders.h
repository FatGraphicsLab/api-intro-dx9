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

#include <d3d9.h>
#include <d3dx9.h>
#include "Framework/DX/dxerr.h"

extern IDirect3DDevice9* GD3DDevice;

#define SafeRelease(x) { if (x) { x->Release(); x = nullptr; } }
#define SafeDelete(x)  { delete x; x = nullptr; }

#if FAT_DEBUG_BUILD
#  ifndef HR
#  define HR(x) \
   { \
     HRESULT hr = (x); \
     if (FAILED(hr)) \
     { \
       DXTRACE_ERR_MSGBOX(L#x, hr); \
	 } \
   }
#  endif
#else
#  ifndef HR
#  define HR(x) (x)
#  endif
#endif

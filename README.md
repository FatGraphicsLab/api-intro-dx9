# api-intro-dx9

 * An sandbox of DirectD3D9 API
 * Init from "Introduction to 3D Game Programming with DirectX 9.0c - A Shader Approach"

## Build

 * open Code/Solutions/VS2017/ApiIntroDX9.sln
 * build & play~

## Platforms

 * WinXP, Win7, Win10
 * VS2017

## Solution Layout

```
[Bin32]           x86 build
[Bin64]           x64 build
[BinTemp]         intermediate build files
[Asset]           asset data
[Code]
  [External]
  [Framework]     mini framework makes dx9 runs~
  [Samples]       a lot of samples
  [Solutions]
    [VS2017]      vs2017 solution files
```

## Dependency

 * D3DX9, extracts from DXSDK
 * dxerr, [http://blogs.msdn.com/b/chuckw/archive/2012/04/24/where-s-dxerr-lib.aspx][1]

[1]:http://blogs.msdn.com/b/chuckw/archive/2012/04/24/where-s-dxerr-lib.aspx

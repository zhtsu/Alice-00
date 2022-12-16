# Alice-00
[![license](https://img.shields.io/github/license/zhtsu/Alice-00)](LICENSE)

[[中文]](README.zhCN.md)

C++ 2D Game Engine with OpenGL.

This is a learning project that made by following TheCherno's Game Engine Series.

Series Playlist: [Game Engine Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)

## Feature
- Only supported on Windows platform
- Batch rendering for 2D
- Entity Component System architecture
- Include a scene editor
- ~~TODO: Using C# as scripting language~~
- Exporting performance data

## Get Start
### Environment
- MinGW-W64 (posix)
- CMake (3.16 or later)

### Download
```bash
git clone https://github.com/zhtsu/Alice-00.git
cd Alice
git submodule init
git submodule update
```
If an error occurs when running following the command. 
```bash
git submodule update
```
You may need to manually download third-party to 3rdparty/ by links that included in .gitmodules file.

### Build
Double-click to run scripts/Build.bat

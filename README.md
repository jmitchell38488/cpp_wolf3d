# cpp_wolf3d

> [!NOTE]  
> This is pre-alpha stage game. There _are_ bugs. There will be performance issues and the game will slow down if you enable vsync or suffer a low framerate.

This ia based on [Creating a DOOM (Wolfenstein) - style 3D Game in Python][yt_wolf3d].

This is an implementation in C++ using the [olcPixelGameEngine][ln-pge] from [One Lone Coder (@javidx9)][yt].

### How to build
This project uses CMake for cross-platform support. Load the project into your favourite IDE and build!

From the CLI:

```bash
# compile it
cmake --build <build_path> --config Debug --target ALL_BUILD -j 14

# run it
build/bin/main<.ext>
```

This game is _not_ optimized to run with [OLC Pixel Game Engine Mobile][olc-pgex-mobile]

### How to play

TBD


[yt_wolf3d]: https://www.youtube.com/watch?v=ECqUrT7IdqQ
[pge]: https://github.com/OneLoneCoder/olcPixelGameEngine/
[yt]: https://www.youtube.com/@javidx9
[olc-pgex-mobile]: https://github.com/Johnnyg63/OLCPGEMobileAndroidStudio
[ln-ma]: https://miniaud.io/
[ln-stb]: https://github.com/nothings/stb/blob/master/stb_image.h
[ln-pgex-font]: https://github.com/gorbit99/OLC-Font
[ln-pgex-ma]: https://github.com/Moros1138/olcPGEX_MiniAudio
[ln-pge]: https://github.com/OneLoneCoder/olcPixelGameEngine
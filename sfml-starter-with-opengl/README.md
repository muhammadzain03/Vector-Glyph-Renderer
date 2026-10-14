# Vector Glyph Renderer

CPSC 453 Homework 2. Draws a command-line string as outlines from the Computer Modern typewriter font (`cmuntt`). Cubic `C` commands are sampled into short lines. The sentence fits in the window when the program starts.

Built on the course SFML + OpenGL 2.1 starter. Audio and network are off. GLAD loads OpenGL.

## Run

From this folder:

```powershell
cmake -B build
cmake --build build
.\build\bin\main.exe "The quick brown fox jumps over the lazy dog"
```

## Controls

- Left / Right: scroll
- Up / Down: zoom

## References

The cubic Bézier sampling follows:

- Guidev, [Bezier Curves Explained](https://www.youtube.com/watch?v=pnYccz1Ha34)
- Freya Holmér, [The Beauty of Bézier Curves](https://www.youtube.com/watch?v=aVwxzDHniEw&t=19s)

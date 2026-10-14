# Vector Glyph Renderer

CPSC 453 Homework 2. Draws a command-line string as outlines from the Computer Modern typewriter font (`cmuntt`). Cubic `C` commands are sampled into short lines. The sentence fits in the window when the program starts.

Built on the course SFML + OpenGL 2.1 starter in `sfml-starter-with-opengl`. Audio and network are off. GLAD loads OpenGL.

## Run

From `sfml-starter-with-opengl`:

```powershell
cmake -B build
cmake --build build
.\build\bin\main.exe "The quick brown fox jumps over the lazy dog"
```

## Controls

- Left / Right: scroll
- Up / Down: zoom

## Fonts

The Computer Modern Unicode fonts are included next to the starter:

- cmunit - Typewriter-Italic
- cmunrm - Serif-Roman
- cmunsi - SansSerif-Oblique
- cmunsl - Serif-RomanSlanted
- cmunss - SansSerif
- cmunti - Serif-Italic
- cmuntt - Typewriter-Regular

This program uses `cmuntt`. More on these fonts: http://canopus.iacp.dvo.ru/~panov/cm-unicode/

## References

The cubic Bézier sampling follows:

- Guidev, [Bezier Curves Explained](https://www.youtube.com/watch?v=pnYccz1Ha34)
- Freya Holmér, [The Beauty of Bézier Curves](https://www.youtube.com/watch?v=aVwxzDHniEw&t=19s)

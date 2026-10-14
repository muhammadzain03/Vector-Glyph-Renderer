#pragma once

#include <string>
#include <vector>

// One character from the font folder.
// commands holds the parsed M/L/C/Z lines. A space has no file, so commands stays empty.

struct Point {
    float x = 0.f;
    float y = 0.f;
};

struct Command {
    char type;                    // e.g., 'M', 'L', 'C' (a single character)
    std::vector<Point> points;    // A list of X, Y coordinates tied to this command
};

struct Glyph {
    char character = ' ';
    std::vector<Command> commands;
};

// cmun.. --> font data folders
// choosing "cmuntt" folder --> Computer Modern Typewriter, the regular style. 
// The italic typewriter font is the separate folder "cmunti".
std::vector<Glyph> loadGlyphs(const std::string& text, const std::string& fontFolder = "../cmuntt");


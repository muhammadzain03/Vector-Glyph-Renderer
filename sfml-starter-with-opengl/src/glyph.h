#pragma once

#include <string>
#include <vector>

// One character from the font folder.
// Lines hold the raw M/L/C/Z text. A space has no file, so lines stay empty.

struct Glyph{
    char character = ' ';
    std::vector<std::string> lines;
};

// cmun --> font data folders
// choosing "cmuntt" folder --> Typewriter font
std::vector<Glyph> loadGlyphs(const std::string& text, const std::string& fontFolder = "../cmuntt");


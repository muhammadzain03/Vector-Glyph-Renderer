#include "glyph.h"

#include <fstream>
#include <iostream>

std::vector<Glyph> loadGlyphs(const std::string &text, const std::string &fontFolder) {
    std::vector<Glyph> glyphs;

    for (char character : text) {
        Glyph glyph;
        glyph.character = character;

        if (character != ' ') {
            const std::string path = fontFolder + "/gly_" + std::to_string(static_cast<int>(character));
            std::ifstream file(path);
            if (!file) {
                std::cerr << "Missing glyph file: " << path << std::endl;
            } else {
                std::string line;
                while (std::getline(file, line)) {
                    if (!line.empty() && line.back() == '\r') {
                        line.pop_back();
                    }
                    if (!line.empty()) {
                        glyph.lines.push_back(line);
                    }
                } 
            }
        }
        glyphs.push_back(glyph);
    }
    return glyphs;
};
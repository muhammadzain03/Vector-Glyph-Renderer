#include "glyph.h"

#include <fstream>
#include <iostream>
#include <sstream>

// The readCommand function below is an amazing function:
// It parses a single string (a line of text like "L 10.5....") and
// converts it into a structured C++ object.
// |-> Representing a drawing instruction or path command like in SVG files.

Command readCommand(const std::string &line) {
    Command command;
    std::stringstream input(line);  // string to stream. Treats the input as tokens
    input >> command.type;      // grabs the very first token from the above line and assigns it to "command.type"
                                // In vector fonts or SVG paths, this is usually a single letter command.
                                // In our case,  M (Move to), L (Line to), or C (Cubic Bezier curve).

    float x = 0.f;
    float y = 0.f;
    while (input >> x >> y) {   // Loop that extracts numbers in pairs (x and y).
        Point point;            // Inside the loop, each x and y pair is packaged into a Point struct and added to the "command.points" vector.
        point.x = x;
        point.y = y;
        command.points.push_back(point);
    }

    return command;     // Once the entire line is parsed, it returns the populated object. 
}

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
                        glyph.commands.push_back(readCommand(line));
                    }
                } 
            }
        }
        glyphs.push_back(glyph);
    }
    return glyphs;
};
#include "glyph.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

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

// cubic bezier. t = 0 is the start, t = 1 is the end.
// p0 is wherever we already are. the C line gives p1 p2 p3.
Point cubicBezier(Point p0, Point p1, Point p2, Point p3, float t) {
    float u = 1.f - t;
    float w0 = u * u * u;
    float w1 = 3.f * u * u * t;
    float w2 = 3.f * u * t * t;
    float w3 = t * t * t;

    Point point;
    point.x = w0 * p0.x + w1 * p1.x + w2 * p2.x + w3 * p3.x;
    point.y = w0 * p0.y + w1 * p1.y + w2 * p2.y + w3 * p3.y;
    return point;
}

// place the letters on one line, chop curves into lines, then shrink
// the sentence so it fits in the window when the program starts.
std::vector<Point> layoutText(const std::vector<Glyph> &glyphs) {
    const float advance = 0.5f; // cmuntt is typewriter so every char (and a space) is the same width
    const int curveSteps = 12;  // 12 looked smooth enough, more than that didn't change much

    std::vector<Point> lines;
    float penX = 0.f;

    for (const Glyph &glyph : glyphs) {
        if (glyph.character == ' ') {
            penX += advance;
            continue;
        }

        Point current;
        Point start;
        bool hasPoint = false;

        for (const Command &command : glyph.commands) {
            if (command.type == 'M' && !command.points.empty()) {
                current = command.points[0];
                start = current;
                hasPoint = true;
            } else if (command.type == 'L' && hasPoint && !command.points.empty()) {
                Point next = command.points[0];
                lines.push_back(Point{current.x + penX, current.y});
                lines.push_back(Point{next.x + penX, next.y});
                current = next;
            } else if (command.type == 'C' && hasPoint && command.points.size() == 3) {
                // opengl only draws straight lines so I just sample the curve
                Point p0 = current;
                Point prev = p0;
                for (int step = 1; step <= curveSteps; ++step) {
                    float t = static_cast<float>(step) / static_cast<float>(curveSteps);
                    Point next = cubicBezier(p0, command.points[0], command.points[1], command.points[2], t);
                    lines.push_back(Point{prev.x + penX, prev.y});
                    lines.push_back(Point{next.x + penX, next.y});
                    prev = next;
                }
                current = command.points[2];
            } else if (command.type == 'Z' && hasPoint) {
                // Z means "close this shape", so line back to the M
                lines.push_back(Point{current.x + penX, current.y});
                lines.push_back(Point{start.x + penX, start.y});
                current = start;
            }
        }

        penX += advance;
    }

    if (lines.empty())
        return lines;

    // box around the whole sentence, then scale it down so it doesn't hit the edges
    float minX = lines[0].x;
    float maxX = lines[0].x;
    float minY = lines[0].y;
    float maxY = lines[0].y;
    for (const Point &point : lines) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }

    float width = std::max(maxX - minX, 0.001f);
    float height = std::max(maxY - minY, 0.001f);
    float scale = 1.6f / std::max(width, height);
    float midX = (minX + maxX) * 0.5f;
    float midY = (minY + maxY) * 0.5f;

    for (Point &point : lines) {
        point.x = (point.x - midX) * scale;
        point.y = (point.y - midY) * scale;
    }

    return lines;
}
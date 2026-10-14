#pragma once
#include <SFML/Graphics.hpp>
#include <glad/gl.h>
#include <vector>
#include <iostream>

class Triangle : public sf::Drawable {
private:

    // 2d control points for the bezier curve
    std::vector<sf::Vector2f> vertices;

    // shader that will be used to render the bezier curve
    sf::Shader triShader;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:

    Triangle(const std::vector<sf::Vector2f>& points) : vertices(points) 
    {
        // loadFromFile is relative to the current directory.
        // src/ works when you run from the project folder.
        // ../../src/ works when the working directory is build/bin.
        const char* vertPaths[] = { "src/triangle.vert", "../../src/triangle.vert" };
        const char* fragPaths[] = { "src/triangle.frag", "../../src/triangle.frag" };
        bool loaded = false;
        for (int i = 0; i < 2 && !loaded; ++i)
            loaded = triShader.loadFromFile(vertPaths[i], fragPaths[i]);
        if (!loaded)
            std::cerr << "Failed to load shader for triangle " << std::endl;
    }
        
    void setVertices(const std::vector<sf::Vector2f>& points) {
        vertices = points;
    }

};
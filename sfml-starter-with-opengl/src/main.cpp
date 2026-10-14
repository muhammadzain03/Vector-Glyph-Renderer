#include <glad/gl.h> // Must be first!
#include <SFML/Graphics.hpp>

#include <atomic>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

#include "glyph.h"
#include "triangle.h"

std::unique_ptr<Triangle> triangle;
std::vector<Point> textLines;

// 1 means "just fits the window". arrows change these.
float zoom = 1.f;
float scroll = 0.f;

void renderingThread(sf::RenderWindow &window, std::atomic<bool> &running, std::atomic<bool> &contextReady) {
    if (!window.setActive(true)) {
        std::cerr << "CRITICAL: Thread failed to claim context!" << std::endl;
        contextReady = true;
        return;
    }
    contextReady = true;

    // Initialize GLAD with SFML's function loader
    if (!gladLoadGL(sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return;
    }

    // Hard verification check:
    if (sf::Shader::isAvailable()) {
        std::cout << "SUCCESS: Shaders are fully available on this thread!" << std::endl;
    } else {
        std::cerr << "ERROR: Context is active on thread, but driver reports NO SHADERS." << std::endl;
    }

    // match OpenGL's -1 to 1 canonical view with the Y axis pointing up.
    // Passing a negative height (-2.f) flips the Y-axis upside down!
    // Now: Top-Left is (-1.0, 1.0) and Bottom-Right is (1.0, -1.0)
    sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(2.f, -2.f));
    window.setView(view);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    while (running) {
        glClear(GL_COLOR_BUFFER_BIT);

        // two points in a row make one line
        if (!textLines.empty()) {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(scroll, 0.f, 0.f);
            glScalef(zoom, zoom, 1.f);

            glColor3f(0.9f, 0.9f, 0.9f); // Gemini helped deciding the colors
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, textLines.data());
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(textLines.size()));
            glDisableClientState(GL_VERTEX_ARRAY);

            glPopMatrix();
        }

        window.display();
    }

    window.setActive(false);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: main \"The quick brown fox jumps over the lazy dog\"\n";
        return 1;
    }

    std::ostringstream sentence;
    for (int i = 1; i < argc; ++i) {
        if (i > 1)
            sentence << ' ';
        sentence << argv[i];
    }

    const std::string text = sentence.str();
    const std::vector<Glyph> glyphs = loadGlyphs(text, "../cmuntt");
    textLines = layoutText(glyphs);

    std::cout << "Rendering: " << text << std::endl;
    std::cout << "Left/Right scrolls, Up/Down zooms\n";
    for (const Glyph &glyph : glyphs) {
        std::cout << " '" << glyph.character << "' " << glyph.commands.size() << " commands\n";
    }

    sf::ContextSettings settings;
    settings.majorVersion = 2;
    settings.minorVersion = 1;
    settings.attributeFlags = sf::ContextSettings::Default;

    sf::RenderWindow window(
        sf::VideoMode({800, 800}),
        "SFML + GLAD (OpenGL 2.1)",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );

    // repeat on so holding an arrow keeps moving
    window.setKeyRepeatEnabled(true);

    // DO NOT LIMIT FRAME RATE! Use elapsed time to control updates.

    // deactivate its OpenGL context
    if (!window.setActive(false)) {
        std::cerr << "Error setting up window, could not deactivate context" << std::endl;
        return -1;
    }

    // assets
    triangle = std::make_unique<Triangle>(
        std::vector<sf::Vector2f>{
            sf::Vector2f(0.8, 0),
            sf::Vector2f(-0.4, 0.693),
            sf::Vector2f(-0.4, -0.693)});

    std::atomic<bool> running(true);
    std::atomic<bool> contextReady(false);

    // Launch the rendering thread
    std::thread thread(renderingThread, std::ref(window), std::ref(running), std::ref(contextReady));

    // CRUCIAL: Pause the main thread until the rendering thread has successfully claimed the context.
    // This prevents pollEvent() from clashing with setActive(true) during initialization.
    while (!contextReady) {
        std::this_thread::yield();
    }

    // the event/logic/whatever loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                running = false;

                // Wait for the rendering thread to safely deactivate and exit
                if (thread.joinable()) {
                    thread.join();
                }

                // Do additional cleanup here before closing the window
                triangle.reset(); // Release the Triangle object
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Left)
                    scroll -= 0.06f;
                if (keyPressed->scancode == sf::Keyboard::Scan::Right)
                    scroll += 0.06f;
                if (keyPressed->scancode == sf::Keyboard::Scan::Up)
                    zoom *= 1.1f;
                if (keyPressed->scancode == sf::Keyboard::Scan::Down)
                    zoom /= 1.1f;

                // don't let it vanish or blow up
                if (zoom < 0.3f)
                    zoom = 0.3f;
                if (zoom > 6.f)
                    zoom = 6.f;
            }
        }
    }
}

#include "triangle.h"

void Triangle::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Shader::bind(&triShader);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    sf::Shader::bind(NULL);
}
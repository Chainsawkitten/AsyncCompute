#include "Base/Renderer.hpp"

int main() {
    Renderer* renderer = Renderer::makeRenderer(Renderer::BACKEND::OpenGL);
    return 0;
}

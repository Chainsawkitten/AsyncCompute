#include "Base/renderer.hpp"

int main() {
    Renderer* renderer = Renderer::makeRenderer(Renderer::BACKEND::OpenGL);
	return 0;
}

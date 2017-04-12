#pragma once

class Renderer {
    public:
        enum class BACKEND{OpenGL, Vulkan};
        static Renderer* makeRenderer(BACKEND backend);
};

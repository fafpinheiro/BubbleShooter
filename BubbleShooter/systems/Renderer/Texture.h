#pragma once
#include <string>

struct SDL_Texture;
struct SDL_Renderer;

enum class TextureScaleMode
{ // this way i dont need to include the SDL in the TextureManager
    Linear,
    Nearest
};

class Texture
{
public:
    Texture();
    ~Texture();

    // Loads texture from file in path
    bool LoadFromFile(SDL_Renderer* renderer, const std::string& path, TextureScaleMode mode = TextureScaleMode::Linear);
    // Get sdl texture
    SDL_Texture* GetSDLTexture() const { return m_Texture; }

private:
    SDL_Texture* m_Texture = nullptr; // SDL Texture
};
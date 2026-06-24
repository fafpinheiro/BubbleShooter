#pragma once

class Renderer;
class TextureManager;
class AudioManager;

class ResourceLoader
{
public:
    // Load all game resources Texs and Audio
    static void LoadAll(Renderer& renderer, TextureManager& textureManager, AudioManager& audioManager);
};
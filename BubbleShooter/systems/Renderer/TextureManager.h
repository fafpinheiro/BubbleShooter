#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Texture.h"
#include "Renderer.h"

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    // Load texture from file if not already loaded
    Texture* LoadTexture(Renderer& renderer, const std::string& id, const std::string& filepath, TextureScaleMode scaleMode = TextureScaleMode::Linear);

    // Get previously loaded texture by ID
    Texture* GetTexture(const std::string& id) const;

    // Free all textures
    void Clear();

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures; // Textures loaded

};
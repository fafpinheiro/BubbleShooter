#include "TextureManager.h"
#include <iostream>
#include <memory>

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

Texture* TextureManager::LoadTexture(Renderer& renderer, const std::string& id, const std::string& filepath, TextureScaleMode scaleMode)
{
    // Already loaded?
    auto tmpTexture =  m_Textures.find(id);
    if (tmpTexture != m_Textures.end())
    {

        return tmpTexture->second.get();
    }

    auto tex = std::make_unique<Texture>();;
    if (!tex->LoadFromFile(renderer.GetSDLRenderer(), filepath, scaleMode))
    {
        std::cerr << "Failed to load texture: " << filepath << "\n";
        //delete tex;
        return nullptr;
    }

    Texture* rawPtr = tex.get();
    m_Textures[id] = std::move(tex);

    return rawPtr;
}

Texture* TextureManager::GetTexture(const std::string& id) const
{
    auto tmpTexture = m_Textures.find(id);
    if (tmpTexture != m_Textures.end())
    {
        return tmpTexture->second.get();
    }
    return nullptr;
}

void TextureManager::Clear()
{
    m_Textures.clear();
}
#include "ResourceLoader.h"
#include <SDL3/SDL.h>
#include <string>
#include "Renderer/TextureManager.h"
#include "Audio/AudioManager.h"

void ResourceLoader::LoadAll(Renderer& renderer, TextureManager& textureManager, AudioManager& audioManager)
{
    std::string path = SDL_GetBasePath();
    std::string texAssetPath = path + "\\assets\\textures\\";

    std::string circlePath = texAssetPath + "ball12.png";
    std::string xPath = texAssetPath + "ball8.png";
    std::string squarePath = texAssetPath + "ball14.png";
    std::string trianglePath = texAssetPath + "ball5.png";
    std::string cannonPath = texAssetPath + "cannon.png";
    std::string emptyPath = texAssetPath + "empty.png";
    std::string restartButtonPath = texAssetPath + "restartButton.png";

    std::string soundAssetPath = path + "\\assets\\sounds\\";

    std::string wallSxPath = soundAssetPath + "wallSX.mp3";
    std::string attachingSxPath = soundAssetPath + "attachingSX.mp3";
    std::string multiPopSxPath = soundAssetPath + "multiPopSX.mp3";
    std::string popSxPath = soundAssetPath + "popSX.mp3";
    std::string shootSxPath = soundAssetPath + "shootingSX.mp3";
    std::string floatingSxPath = soundAssetPath + "floatingSX.mp3";
    std::string musicPath = soundAssetPath + "elevatorMusic.mp3";

    // Load textures
    textureManager.LoadTexture(renderer, "bubble_circle", circlePath, TextureScaleMode::Nearest);
    textureManager.LoadTexture(renderer, "bubble_x", xPath, TextureScaleMode::Nearest);
    textureManager.LoadTexture(renderer, "bubble_square", squarePath, TextureScaleMode::Nearest);
    textureManager.LoadTexture(renderer, "bubble_triangle", trianglePath, TextureScaleMode::Nearest);
    textureManager.LoadTexture(renderer, "cannon", cannonPath, TextureScaleMode::Nearest);
    textureManager.LoadTexture(renderer, "empty", emptyPath, TextureScaleMode::Nearest);
    textureManager.LoadTexture(renderer, "restartButton", restartButtonPath, TextureScaleMode::Nearest);

    // Load sounds
    audioManager.LoadSound("wallSX", wallSxPath);
    audioManager.LoadSound("attachingSX", attachingSxPath);
    audioManager.LoadSound("multiPopSX", multiPopSxPath);
    audioManager.LoadSound("popSX", popSxPath);
    audioManager.LoadSound("shootingSX", shootSxPath);
    audioManager.LoadSound("floatingSX", floatingSxPath);

    // Load music
    audioManager.LoadMusic("elevatorMusic", musicPath);
}

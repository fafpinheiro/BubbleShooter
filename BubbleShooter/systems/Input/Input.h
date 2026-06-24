#pragma once
#include <unordered_map>
#include <SDL3/SDL.h>
//struct SDL_Keycode;

enum class Key
{
    Left,
    Right,
    Space,
    Escape
};

enum class MouseButton
{
    Left,
    Right,
    Middle
};

class Input
{
public:
    Input();
    ~Input();

    void Update();

    // Gets if key is down
    bool IsKeyDown(Key key) const;
    // Gets if key was clicked
    bool IsKeyPressed(Key key) const;

    // Gets if button is down
    bool IsMouseButtonDown(MouseButton button) const;
    // Gets if button was clicked
    bool IsMouseButtonPressed(MouseButton button) const;

    float GetMouseX() const { return m_MouseX; }
    float GetMouseY() const { return m_MouseY; }

    bool QuitRequested() const { return m_Quit; }

private:
    std::unordered_map<Key, bool> m_KeyDown;                    // Register Holdings
    std::unordered_map<Key, bool> m_KeyPressed;                 // Register clicks

    std::unordered_map<MouseButton, bool> m_MouseDown;          // Register Holdings
    std::unordered_map<MouseButton, bool> m_MousePressed;       // Register clicks

    float m_MouseX = 0;                                         // X mouse pos
    float m_MouseY = 0;                                         // Y mouse pos

    bool m_Quit = false;

    // Convert SDL keyboard button code, to game standard code
    Key SDLKeyToKey(SDL_Keycode key);
    // Convert SDL mouse button code, to game standard code
    MouseButton SDLMouseToButton(Uint8 button);
};

#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::Update()
{
    m_KeyPressed.clear();
    m_MousePressed.clear();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

        // Quit Game event
        if (event.type == SDL_EVENT_QUIT)
        {
            m_Quit = true;
        }

        // Keyboard
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            Key key = SDLKeyToKey(event.key.key);

            if (!m_KeyDown[key])
            {
                m_KeyPressed[key] = true;
            }

            m_KeyDown[key] = true;
        }

        if (event.type == SDL_EVENT_KEY_UP)
        {
            Key key = SDLKeyToKey(event.key.key);
            m_KeyDown[key] = false;
        }

        // Mouse move
        if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
            m_MouseX = event.motion.x;
            m_MouseY = event.motion.y;
        }

        // Mouse button
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            MouseButton button = SDLMouseToButton(event.button.button);

            if (!m_MouseDown[button])
            {
                m_MousePressed[button] = true;
            }
            m_MouseDown[button] = true;
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            MouseButton button = SDLMouseToButton(event.button.button);
            m_MouseDown[button] = false;
        }
    }
}

bool Input::IsKeyDown(Key key) const
{
    auto tmpKey = m_KeyDown.find(key);
    if (tmpKey != m_KeyDown.end())
        return tmpKey->second;
    return 0; // safe if missing
}

bool Input::IsKeyPressed(Key key) const
{
    auto tmpKey = m_KeyPressed.find(key);
    if (tmpKey != m_KeyPressed.end())
        return tmpKey->second;
    return 0; // safe if missing
}

bool Input::IsMouseButtonDown(MouseButton button) const
{
    auto tmpButton = m_MouseDown.find(button);
    if (tmpButton != m_MouseDown.end())
        return tmpButton->second;
    return 0; // safe if missing
}

bool Input::IsMouseButtonPressed(MouseButton button) const
{
    auto tmpButton = m_MousePressed.find(button);
    if (tmpButton != m_MousePressed.end())
        return tmpButton->second;
    return 0; // safe if missing
}

Key Input::SDLKeyToKey(SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_LEFT: return Key::Left;
        case SDLK_RIGHT: return Key::Right;
        case SDLK_SPACE: return Key::Space;
        case SDLK_ESCAPE: return Key::Escape;
        default: return Key::Escape;
    }
}

MouseButton Input::SDLMouseToButton(Uint8 button) {
    switch (button)
    {
        case SDL_BUTTON_LEFT: return MouseButton::Left;
        case SDL_BUTTON_RIGHT: return MouseButton::Right;
        case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
        default: return MouseButton::Left;
    }
}
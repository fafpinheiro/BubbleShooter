
Build and Run:

Used libs:
- SDL3, SDL_image, SDL_mixer

Build System:
CMAKE:
- Generates platform-specific build files
- Links the libraries (including the creation of the dll files in the deployed folder)
- The builds will go to out/build/...


Future Improvements:

- Add animations for bubble popping and falling
- Implement level system with increasing difficulty
- Add score tracking and UI display
- Use the SDL_ttf and a new class encapsulation for the creation of the UI

# CMakeList.txt : CMake project for BubbleShooter, include source and define
# project specific logic here.
#

#find_package(SDL3 REQUIRED)
#find_package(SDL3_image REQUIRED)
#find_package(SDL3_mixer REQUIRED)

# Add source to this project's executable.
add_executable (BubbleShooter "BubbleShooter.cpp" "BubbleShooter.h"   "systems/Renderer/Renderer.h" "systems/Renderer/Renderer.cpp" "systems/Window/Window.h" "systems/Window/Window.cpp" "core/Game.h" "core/Game.cpp" "systems/Renderer/Texture.h" "systems/Renderer/Texture.cpp" "systems/Renderer/TextureManager.h" "systems/Renderer/TextureManager.cpp" "game/Level.h" "game/Level.cpp" "systems/Input/Input.h" "systems/Input/Input.cpp" "entities/Cannon.h" "entities/Cannon.cpp" "systems/Timer/Time.h" "systems/Timer/Time.cpp"  "entities/Bubble.h" "entities/Bubble.cpp" "systems/Audio/AudioManager.h" "systems/Audio/AudioManager.cpp" "systems/SDL/SDLManager.h" "systems/SDL/SDLManager.cpp" "systems/Action/DelayedActionManager.h" "systems/Action/DelayedActionManager.cpp" "systems/Audio/Audio.h" "systems/Audio/Audio.cpp" "systems/Resources/ResourceLoader.h" "systems/Resources/ResourceLoader.cpp"  )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BubbleShooter PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
#target_link_libraries(BubbleShooter PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer)
#target_include_directories(BubbleShooter PRIVATE "ext/")
target_include_directories(BubbleShooter PRIVATE "assets/")
target_include_directories(BubbleShooter PRIVATE "entities/")
target_include_directories(BubbleShooter PRIVATE "game/")
target_include_directories(BubbleShooter PRIVATE "systems/")

## include ext

# SDL path (relative to project)
set(SDL3_PATH ${CMAKE_SOURCE_DIR}/BubbleShooter/ext/SDL3)
# Include headers
target_include_directories(BubbleShooter PRIVATE
    ${SDL3_PATH}/include
)
# Link SDL library
target_link_directories(BubbleShooter PRIVATE
    ${SDL3_PATH}/lib/x64
)

target_link_libraries(BubbleShooter PRIVATE
    SDL3
)
###
set(SDL3_IMAGE_PATH ${CMAKE_SOURCE_DIR}/BubbleShooter/ext/SDL3_image)
# Include headers
target_include_directories(BubbleShooter PRIVATE
    ${SDL3_IMAGE_PATH}/include
)
# Link SDL library
target_link_directories(BubbleShooter PRIVATE
    ${SDL3_IMAGE_PATH}/lib/x64
)

target_link_libraries(BubbleShooter PRIVATE
    SDL3_image
)
###
set(SDL3_MIXER_PATH ${CMAKE_SOURCE_DIR}/BubbleShooter/ext/SDL3_mixer)
# Include headers
target_include_directories(BubbleShooter PRIVATE
    ${SDL3_MIXER_PATH}/include
)
# Link SDL library
target_link_directories(BubbleShooter PRIVATE
    ${SDL3_MIXER_PATH}/lib/x64
)

target_link_libraries(BubbleShooter PRIVATE
    SDL3_mixer
)

###
### Include headers
##target_include_directories(BubbleShooter PRIVATE
##    ${CMAKE_SOURCE_DIR}/BubbleShooter/ext/glm
##)

## Build ext copy
# Copy SDL3.dll after build
add_custom_command(TARGET BubbleShooter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL3_PATH}/lib/x64/SDL3.dll
    $<TARGET_FILE_DIR:BubbleShooter>
)

# Copy SDL3_image.dll after build
add_custom_command(TARGET BubbleShooter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL3_IMAGE_PATH}/lib/x64/SDL3_image.dll
    $<TARGET_FILE_DIR:BubbleShooter>
)

# Copy SDL3_mixer.dll after build
add_custom_command(TARGET BubbleShooter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL3_MIXER_PATH}/lib/x64/SDL3_mixer.dll
    $<TARGET_FILE_DIR:BubbleShooter>
)

# Copy assets folder after building
add_custom_command(TARGET BubbleShooter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:BubbleShooter>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/BubbleShooter/assets"
            "$<TARGET_FILE_DIR:BubbleShooter>/assets"
)



### Copy SDL3.dll after build
##add_custom_command(TARGET BubbleShooter POST_BUILD
##    COMMAND ${CMAKE_COMMAND} -E copy_if_different
##    $<TARGET_FILE:SDL3::SDL3>
##    $<TARGET_FILE_DIR:BubbleShooter>
##)
##
### Copy SDL3_image.dll after build
##add_custom_command(TARGET BubbleShooter POST_BUILD
##    COMMAND ${CMAKE_COMMAND} -E copy_if_different
##    $<TARGET_FILE:SDL3_image::SDL3_image>
##    $<TARGET_FILE_DIR:BubbleShooter>
##)
##
### Copy SDL3_mixer.dll after build
##add_custom_command(TARGET BubbleShooter POST_BUILD
##    COMMAND ${CMAKE_COMMAND} -E copy_if_different
##    $<TARGET_FILE:SDL3_mixer::SDL3_mixer>
##    $<TARGET_FILE_DIR:BubbleShooter>
##)
##
### Copy assets folder after building
##add_custom_command(TARGET BubbleShooter POST_BUILD
##    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:BubbleShooter>/assets"
##    COMMAND ${CMAKE_COMMAND} -E copy_directory
##            "${CMAKE_SOURCE_DIR}/BubbleShooter/assets"
##            "$<TARGET_FILE_DIR:BubbleShooter>/assets"
##)
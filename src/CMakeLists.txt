add_library(lib1 STATIC util.cpp draw.cpp Background.cpp AudioPlayer.cpp Text.cpp HighscoreTable.cpp Stage.cpp App.cpp)
# target_link_libraries(lib1 PRIVATE "libSDL2_image-2.0.so" "libSDL2_mixer-2.0.so")
target_link_libraries(lib1 PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)
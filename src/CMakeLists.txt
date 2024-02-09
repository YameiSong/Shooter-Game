add_library(lib1 STATIC draw.cpp init.cpp input.cpp util.cpp AudioPlayer.cpp Text.cpp Stage.cpp)
# target_link_libraries(lib1 PRIVATE "libSDL2_image-2.0.so" "libSDL2_mixer-2.0.so")
target_link_libraries(lib1 PRIVATE SDL2 SDL2_image SDL2_ttf SDL2_mixer)
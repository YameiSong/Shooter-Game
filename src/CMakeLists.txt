add_library(lib1 STATIC draw.cpp init.cpp input.cpp util.cpp stage.cpp)
target_link_libraries(lib1 PRIVATE "libSDL2_image-2.0.so")
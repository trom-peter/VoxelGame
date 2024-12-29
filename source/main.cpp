#include "voxel_game.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

int main(int argc, char** argv) {
    VoxelGame game;

    if (game.init() != 0) {
        return -1;
    }

    game.run();

    return game.quit();
}

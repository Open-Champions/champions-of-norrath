// Host check only; not a build input.
#include "collide.cpp"
int main() {
    unsigned char block[5][16];
    for (int round = 0; round < 3; ++round) {
        for (int i = 0; i < 5; ++i) for (int j = 0; j < 16; ++j) block[i][j] = static_cast<unsigned char>(0x11 * (i + 1) + j + round);
        unsigned char before[5][16];
        for (int i = 0; i < 5; ++i) for (int j = 0; j < 16; ++j) before[i][j] = block[i][j];
        if (Collide__12VISkinSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide(block[0], block[1], block[2], block[3], block[4]) != 0) return 1;
        for (int i = 0; i < 5; ++i) for (int j = 0; j < 16; ++j) if (block[i][j] != before[i][j]) return 2;
    }
    return 0;
}

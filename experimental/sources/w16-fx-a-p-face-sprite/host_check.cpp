// Host check only; not a build input.
#include "p_face_sprite.cpp"
#include <cstring>
unsigned char singleBuffer[1024 * 32 + 64];
unsigned nmSingle;
int main() {
    Point3 p = {1.0f, 2.0f, 3.0f};
    int texture = 0;
    std::memset(singleBuffer, 0xcd, sizeof singleBuffer);
    nmSingle = 2;
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&p, 4.0f, 5.0f, 0x1ff, 2, 3, -1, &texture, -7, 0x12345);
    if (nmSingle != 3) return 1;
    const unsigned char* r = singleBuffer + 64;
    float f[5];
    std::memcpy(f, r, 12); std::memcpy(f + 3, r + 12, 4); std::memcpy(f + 4, r + 16, 4);
    if (f[0] != 1.0f || f[1] != 2.0f || f[2] != 3.0f || f[3] != 5.0f || f[4] != 4.0f) return 2;
    if (r[20] != 0xff || r[21] != 2 || r[22] != 3 || r[23] != 0xff) return 3;
    short s[2]; std::memcpy(s, r + 28, 4);
    if (s[0] != -7 || s[1] != 0x2345) return 4;
    for (int i = 0; i < 32; ++i) if (singleBuffer[32 + i] != 0xcd || singleBuffer[96 + i] != 0xcd) return 5;
    nmSingle = 1024;
    std::memset(singleBuffer, 0xcd, sizeof singleBuffer);
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&p, 4.0f, 5.0f, 0, 0, 0, 0, &texture, 0, 0);
    if (nmSingle != 1024 || singleBuffer[1024 * 32] != 0xcd || singleBuffer[1023 * 32] != 0xcd) return 6;
    return 0;
}

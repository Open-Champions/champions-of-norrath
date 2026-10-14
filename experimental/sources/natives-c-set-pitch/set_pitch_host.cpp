#include <cstdio>
#include <cstring>

extern "C" void SetPitchAtLevel__6Cameraii(void* camera, int level, int pitch);

static unsigned short half(const unsigned char* bytes, unsigned offset) {
    return static_cast<unsigned short>(bytes[offset] | (bytes[offset + 1] << 8));
}

static void put_int(unsigned char* bytes, unsigned offset, int value) {
    auto* source = reinterpret_cast<unsigned char*>(&value);
    for (int i = 0; i < 4; ++i) bytes[offset + i] = source[i];
}

int main() {
    alignas(8) unsigned char camera[256];
    std::memset(camera, 0x11, sizeof camera);
    put_int(camera, 84, 2);
    SetPitchAtLevel__6Cameraii(camera, 2, 300);
    if (half(camera, 108 + 4) != 300) return 1;
    if (half(camera, 106) != 300) return 2;
    SetPitchAtLevel__6Cameraii(camera, 1, 400);
    if (half(camera, 108 + 2) != 400) return 3;
    if (half(camera, 106) != 300) return 4;
    SetPitchAtLevel__6Cameraii(camera, 0, 70000);
    if (half(camera, 108) != static_cast<unsigned short>(70000)) return 5;
    if (half(camera, 106) != 300) return 6;
    SetPitchAtLevel__6Cameraii(camera, 2, -2);
    if (half(camera, 108 + 4) != 0xfffe) return 7;
    if (half(camera, 106) != 0xfffe) return 8;
    std::printf("set pitch passed\n");
    return 0;
}

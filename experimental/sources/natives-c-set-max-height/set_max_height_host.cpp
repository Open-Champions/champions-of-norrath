#include <cstdio>
#include <cstring>

extern "C" void SetMaxHeight__6Cameraf(void* camera, float height);

static int fail(int code) {
    std::printf("set max height failed %d\n", code);
    return code;
}

int main() {
    alignas(8) unsigned char camera[256];
    std::memset(camera, 0x5a, sizeof camera);
    SetMaxHeight__6Cameraf(camera, 12.5f);
    float stored = 0.f;
    std::memcpy(&stored, camera + 164, 4);
    if (stored != 12.5f) return fail(1);
    for (int i = 0; i < 256; ++i) {
        if (i >= 164 && i < 168) continue;
        if (camera[i] != 0x5a) return fail(2);
    }
    SetMaxHeight__6Cameraf(camera, -4.f);
    std::memcpy(&stored, camera + 164, 4);
    if (stored != -4.f) return fail(3);
    std::printf("set max height passed\n");
    return 0;
}

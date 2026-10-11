#include <cstdio>
#include <cstring>

extern "C" void DisableZooming__6Camera(void* camera);

static void put_int(unsigned char* bytes, unsigned offset, int value) {
    std::memcpy(bytes + offset, &value, 4);
}

static void put_float(unsigned char* bytes, unsigned offset, float value) {
    std::memcpy(bytes + offset, &value, 4);
}

static int get_int(const unsigned char* bytes, unsigned offset) {
    int value = 0;
    std::memcpy(&value, bytes + offset, 4);
    return value;
}

static float get_float(const unsigned char* bytes, unsigned offset) {
    float value = 0.f;
    std::memcpy(&value, bytes + offset, 4);
    return value;
}

int main() {
    alignas(8) unsigned char camera[320];
    std::memset(camera, 0x3c, sizeof camera);
    put_int(camera, 84, 0);
    put_float(camera, 176, 10.f);
    put_float(camera, 180, 30.f);
    put_float(camera, 184, 50.f);
    put_float(camera, 188, 7.f);
    put_float(camera, 192, 80.f);
    DisableZooming__6Camera(camera);
    if (get_int(camera, 80) != 1) return 1;
    if (get_int(camera, 84) != 1) return 2;
    if (get_int(camera, 88) != 0) return 3;
    if (get_float(camera, 144) != 1.f) return 4;
    if (get_float(camera, 160) != 1.f) return 5;
    if (get_float(camera, 140) != 30.f) return 6;
    if (get_float(camera, 216) != 0.5f) return 7;
    if (get_float(camera, 252) != 80.f) return 8;
    if (get_float(camera, 176) != 10.f) return 9;
    if (get_float(camera, 188) != 7.f) return 10;

    std::memset(camera, 0x3c, sizeof camera);
    put_int(camera, 84, 1);
    put_float(camera, 176, 10.f);
    put_float(camera, 180, 30.f);
    put_float(camera, 184, 50.f);
    put_float(camera, 192, 8.f);
    put_float(camera, 200, 4.f);
    DisableZooming__6Camera(camera);
    if (get_int(camera, 88) != 1) return 11;
    if (get_float(camera, 140) != 30.f) return 12;
    if (get_float(camera, 216) != 0.5f) return 13;
    if (get_float(camera, 252) != 8.f) return 14;
    std::printf("disable zoom passed\n");
    return 0;
}

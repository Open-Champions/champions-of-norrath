#include <cstdio>
#include <cstring>

extern "C" void SetDistanceAtLevel__6Cameraif(void* camera, int level, float distance);

static void put_int(unsigned char* bytes, unsigned offset, int value) {
    std::memcpy(bytes + offset, &value, 4);
}

static void put_float(unsigned char* bytes, unsigned offset, float value) {
    std::memcpy(bytes + offset, &value, 4);
}

static float get_float(const unsigned char* bytes, unsigned offset) {
    float value = 0.f;
    std::memcpy(&value, bytes + offset, 4);
    return value;
}

int main() {
    alignas(8) unsigned char camera[320];
    std::memset(camera, 0x22, sizeof camera);
    put_int(camera, 84, 0);
    put_float(camera, 176, 10.f);
    put_float(camera, 180, 20.f);
    SetDistanceAtLevel__6Cameraif(camera, 1, 40.f);
    if (get_float(camera, 180) != 40.f) return 1;
    if (get_float(camera, 176) != 10.f) return 2;
    if (get_float(camera, 140) == 40.f) return 3;

    std::memset(camera, 0x22, sizeof camera);
    put_int(camera, 84, 1);
    put_int(camera, 88, 0);
    put_float(camera, 176, 10.f);
    put_float(camera, 180, 20.f);
    put_float(camera, 184, 50.f);
    put_float(camera, 144, 2.f);
    put_float(camera, 188, 5.f);
    put_float(camera, 192, 9.f);
    SetDistanceAtLevel__6Cameraif(camera, 1, 40.f);
    if (get_float(camera, 180) != 40.f) return 4;
    if (get_float(camera, 140) != 40.f) return 5;
    if (get_float(camera, 216) != 0.25f) return 6;
    if (get_float(camera, 252) != 9.f) return 7;
    if (get_float(camera, 176) != 10.f) return 8;

    std::memset(camera, 0x22, sizeof camera);
    put_int(camera, 84, 0);
    put_int(camera, 88, 1);
    put_float(camera, 176, 8.f);
    put_float(camera, 184, 18.f);
    put_float(camera, 144, 0.5f);
    put_float(camera, 188, 3.f);
    put_float(camera, 192, 11.f);
    SetDistanceAtLevel__6Cameraif(camera, 0, 8.f);
    if (get_float(camera, 176) != 8.f) return 9;
    if (get_float(camera, 216) != 1.f) return 10;
    if (get_float(camera, 252) != 7.f) return 11;
    std::printf("set distance passed\n");
    return 0;
}

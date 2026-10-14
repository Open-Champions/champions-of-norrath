#include <cstring>
static unsigned char objects[3][16];
static int mode;
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "orc");
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char*, void** list, int limit) {
    if (limit != 128) {
        list[0] = 0;
        return;
    }
    if (mode == 0) {
        list[0] = 0;
        return;
    }
    list[0] = objects[0];
    list[1] = objects[1];
    list[2] = objects[2];
    list[3] = 0;
}
extern "C" int amxCountLiveObjects__FP5__amxPi(void*, int*);
static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    int params[2] = {4, 1};
    mode = 0;
    if (amxCountLiveObjects__FP5__amxPi(0, params) != 0) return 1;
    mode = 1;
    store_u32(objects[0] + 12, 0x28u);
    store_u32(objects[1] + 12, 0x20u);
    store_u32(objects[2] + 12, 0x00u);
    if (amxCountLiveObjects__FP5__amxPi(0, params) != 2) return 2;
    store_u32(objects[0] + 12, 0xffu);
    store_u32(objects[1] + 12, 0x08u);
    store_u32(objects[2] + 12, 0x28u);
    if (amxCountLiveObjects__FP5__amxPi(0, params) != 1) return 3;
    return 0;
}

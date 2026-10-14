#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" int amxStartDialog__FP5__amxPi(void*, int*);
extern "C" char levelName[322] = {};

static char loaded_path[80];
static char found_name[128];
static void* loaded_token;
static unsigned char found[16];
static char dialog_name[160];
static void* packet;
static int first;
static int second;
static int marker = 0x51;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "welcome");
    return 0;
}

extern "C" void* lumpLoad__FPc(char* path) {
    std::snprintf(loaded_path, sizeof loaded_path, "%s", path);
    return loaded_token;
}

extern "C" void* lumpFind__FPvPc(void* loaded, char* name) {
    if (loaded != loaded_token) return nullptr;
    std::snprintf(found_name, sizeof found_name, "%s", name);
    return found;
}

extern "C" void DLG_StartDialog__FPcP9DlgPacketii(char* name, void* body, int a, int b) {
    std::snprintf(dialog_name, sizeof dialog_name, "%s", name);
    packet = body;
    first = a;
    second = b;
}

int main() {
    std::snprintf(levelName + 81, 16, "%s", "town");
    void* expected = &marker;
    std::memcpy(found + 4, &expected, sizeof expected);
    loaded_token = &marker;
    int params[4] = {12, 5, 6, 7};
    if (amxStartDialog__FP5__amxPi(nullptr, params) != 0) return 1;
    if (std::strcmp(loaded_path, "town.lmp") != 0) return 2;
    if (std::strcmp(found_name, "welcome.dbin") != 0) return 3;
    if (std::strcmp(dialog_name, "welcome") != 0 || packet != expected) return 4;
    if (first != 6 || second != 7) return 5;
    std::printf("start dialog passed\n");
    return 0;
}

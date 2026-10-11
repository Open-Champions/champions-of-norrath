#include <cstdio>
#include <cstring>
extern "C" int amxHideMonster__FP5__amxPi(void*, int*);
static unsigned char first[571];
static unsigned char second[571];
static int removes = 0;
static void* removed[4];
static int limit_seen = 0;
extern "C" int amx_GetAddr(void*, int address, void** physical) {
    static char packed[4];
    if (address != 9) return 1;
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "bat");
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit) {
    if (std::strcmp(name, "bat") != 0) list[0] = nullptr;
    limit_seen = limit;
    list[0] = first;
    list[1] = second;
    list[2] = nullptr;
}
extern "C" void objectRemoveFromGrid__FP10GameObject(void* object) {
    if (removes < 4) removed[removes] = object;
    removes += 1;
    if (removes > 4) {
        first[12] = 0;
        second[12] = 0;
    }
}

int main() {
    std::memset(first, 0, sizeof first);
    std::memset(second, 0, sizeof second);
    int cells[] = {4, 9};
    first[12] = 0x20;
    second[12] = 0;
    second[570] = 0x1;
    if (amxHideMonster__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (removes != 2 || removed[0] != first || removed[1] != second) return 2;
    if (first[570] != 0x8 || second[570] != 0x1) return 3;
    if (limit_seen != 512) return 4;
    std::puts("amxHideMonster passed");
    return 0;
}

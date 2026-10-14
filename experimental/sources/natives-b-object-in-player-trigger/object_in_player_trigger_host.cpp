#include <cstdio>
#include <cstring>
extern "C" int amxObjectInPlayerTrigger__FP5__amxPi(void*, int*);
static int named_marker = 1;
static int missing = 0;
static char class_name[] = "Trigger";
static void* type_info[] = {class_name};
static unsigned char skipped[112];
static unsigned char hit[112];
static unsigned char table[160];
static int calls = 0;
static void* self_seen = nullptr;
static int class_limit = 0;
static int hit_result = 1;
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "door");
    return 0;
}
extern "C" void* objectFindObject__FPc(char* name) {
    if (missing || std::strcmp(name, "door") != 0) return nullptr;
    return &named_marker;
}
extern "C" void* __tf7Trigger(void) { return type_info; }
extern "C" void objectFindObjectByClass__FPcPP10GameObjecti(char* name, void** list, int limit) {
    class_limit = limit;
    if (std::strcmp(name, "Trigger") != 0) class_limit = -1;
    list[0] = skipped;
    list[1] = hit;
    list[2] = nullptr;
}
static int call_trigger(void* self, void* named) {
    calls += 1;
    self_seen = self;
    if (named != &named_marker) return -1;
    return self == hit + 6 ? hit_result : 0;
}

int main() {
    std::memset(skipped, 0, sizeof skipped);
    std::memset(hit, 0, sizeof hit);
    std::memset(table, 0, sizeof table);
    skipped[104] = 0xff;
    skipped[105] = 0xff;
    skipped[106] = 0xff;
    skipped[107] = 0xff;
    *reinterpret_cast<void**>(hit + 72) = table;
    const short adjust = 6;
    std::memcpy(table + 136, &adjust, sizeof adjust);
    int (*fn)(void*, void*) = call_trigger;
    std::memcpy(table + 140, &fn, sizeof fn);
    int cells[] = {4, 2};
    missing = 1;
    if (amxObjectInPlayerTrigger__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (calls != 0) return 2;
    missing = 0;
    hit_result = 0;
    if (amxObjectInPlayerTrigger__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (calls != 1 || self_seen != hit + 6 || class_limit != 50) return 4;
    hit_result = 1;
    if (amxObjectInPlayerTrigger__FP5__amxPi(nullptr, cells) != 1) return 5;
    if (calls != 2) return 6;
    std::puts("amxObjectInPlayerTrigger passed");
    return 0;
}

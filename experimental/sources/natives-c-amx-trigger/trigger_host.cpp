#include <cstdio>
#include <cstring>

extern "C" int amxTrigger__FP5__amxPi(void*, int*);

static unsigned char objects[2][96];
static unsigned char tables[2][40];
static void* selves[4];
static int arguments[4];
static int calls;
static int asked;
static int empty;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "gate");
    return 0;
}

extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int count) {
    asked = std::strcmp(name, "gate") == 0 ? count : -1;
    if (empty || asked < 0) {
        list[0] = nullptr;
        return;
    }
    list[0] = objects[0];
    list[1] = objects[1];
    list[2] = nullptr;
}

static void on_trigger(void* self, int argument) {
    selves[calls] = self;
    arguments[calls] = argument;
    calls += 1;
}

static void arm(int index, short adjust) {
    void* table = tables[index];
    std::memcpy(objects[index] + 72, &table, sizeof table);
    std::memcpy(tables[index] + 24, &adjust, sizeof adjust);
    auto function = on_trigger;
    std::memcpy(tables[index] + 28, &function, sizeof function);
}

int main() {
    int params[3] = {8, 1, 9};
    empty = 1;
    if (amxTrigger__FP5__amxPi(nullptr, params) != 0 || calls != 0 || asked != 100) return 1;
    empty = 0;
    arm(0, -4);
    arm(1, 8);
    if (amxTrigger__FP5__amxPi(nullptr, params) != 0) return 2;
    if (calls != 2) return 3;
    if (selves[0] != objects[0] - 4 || arguments[0] != 9) return 4;
    if (selves[1] != objects[1] + 8 || arguments[1] != 9) return 5;
    std::printf("trigger passed\n");
    return 0;
}

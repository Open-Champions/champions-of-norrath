#include "mc_reset_auto_check_point_save.cpp"

#include <cstdio>
#include <initializer_list>
#include <cstring>

int asyncMode, saveCheckPointCount;
static int active;
static char seen[16];

extern "C" int engineTaskActive__FPc(const char* name) {
    std::strcpy(seen, name);
    return active;
}

int main() {
    char arena[] = "arena1", other[] = "cave";
    saveCheckPointCount = 55;
    asyncMode = 0; active = 0;
    MC_ResetAutoCheckPointSave__FPc(other);
    if (saveCheckPointCount != 1 || std::strcmp(seen, "deathTask") != 0) return 1;
    MC_ResetAutoCheckPointSave__FPc(arena);
    if (saveCheckPointCount != 10) return 2;
    active = 1; saveCheckPointCount = 55;
    MC_ResetAutoCheckPointSave__FPc(arena);
    if (saveCheckPointCount != 55) return 3;
    active = 0;
    for (int mode : {3, 7}) {
        asyncMode = mode;
        MC_ResetAutoCheckPointSave__FPc(arena);
        if (saveCheckPointCount != 55) return 4;
    }
    asyncMode = 4;
    MC_ResetAutoCheckPointSave__FPc(arena);
    if (saveCheckPointCount != 10) return 5;
    std::puts("mc reset auto check point save passed");
    return 0;
}

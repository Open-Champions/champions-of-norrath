#include <cstdarg>
#include <cstring>
extern "C" char levelName[322] = {};
static char lump[80];
static char cut[80];
static void* scene = levelName;
static void* started_scene;
static char started_cut[80];
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "gate");
    return 0;
}
extern "C" int sprintf(char* destination, const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    if (std::strcmp(format, "%s.lmp") == 0) {
        const char* stem = va_arg(args, const char*);
        if (stem != levelName + 81) destination[0] = 0;
        else std::strcpy(destination, "town.lmp");
        std::strcpy(lump, destination);
    } else if (std::strcmp(format, "%s.cut") == 0) {
        const char* stem = va_arg(args, const char*);
        if (std::strcmp(stem, "gate") != 0) destination[0] = 0;
        else std::strcpy(destination, "gate.cut");
        std::strcpy(cut, destination);
    } else {
        destination[0] = 0;
    }
    va_end(args);
    return 0;
}
extern "C" void* lumpFindResource__FPcT0(char* got_lump, char* got_cut) {
    if (std::strcmp(got_lump, "town.lmp") != 0 || std::strcmp(got_cut, "gate.cut") != 0) return 0;
    return scene;
}
extern "C" void cutStartScene__FP9_cutscenePc(void* got_scene, char* got_cut) {
    started_scene = got_scene;
    std::strcpy(started_cut, got_cut);
}
extern "C" int amxCutscene__FP5__amxPi(void*, int*);
int main() {
    std::strcpy(levelName + 81, "town");
    int params[2] = {4, 9};
    if (amxCutscene__FP5__amxPi(0, params) != 0) return 1;
    if (std::strcmp(lump, "town.lmp") != 0 || std::strcmp(cut, "gate.cut") != 0) return 2;
    if (started_scene != scene || std::strcmp(started_cut, "gate.cut") != 0) return 3;
    return 0;
}

#include <cstdio>
#include <cstring>
extern "C" int amxNewWorldParameter__FP5__amxPi(void*, int*);
static unsigned char object[64];
extern "C" unsigned char* currentRunningObject = object;
static int kicks = 0;
static char* world = nullptr;
extern "C" void gameNetKick__Fi(int code) {
    kicks += code == 0 ? 1 : 100;
}
extern "C" void doNewWorld__FPc(char* name) { world = name; }

int main() {
    char music[] = "music=a";
    char level[] = "level=zone";
    char other[] = "level=other";
    char* entries[] = {music, level, other, nullptr};
    char** list = entries;
    std::memcpy(object + 44, &list, sizeof list);
    if (amxNewWorldParameter__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    if (kicks != 1 || world == nullptr || std::strcmp(world, "zone") != 0) return 2;
    char music_only[] = "music=a";
    char* empty_entries[] = {music_only, nullptr};
    char** empty = empty_entries;
    std::memcpy(object + 44, &empty, sizeof empty);
    if (amxNewWorldParameter__FP5__amxPi(nullptr, nullptr) != 0) return 3;
    if (kicks != 1) return 4;
    std::puts("amxNewWorldParameter passed");
    return 0;
}

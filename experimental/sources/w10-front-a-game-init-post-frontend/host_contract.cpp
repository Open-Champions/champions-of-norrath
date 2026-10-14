#include "game_init_post_frontend.cpp"

#include <cstdio>
#include <cstring>
#include <string>

unsigned char globalIcons[152];
static std::string events;
static unsigned char entries[10][8];
static int lumpToken;

extern "C" {
void* lumpLoad__FPc(const char* name) {
    events += std::string("load ") + name + ";";
    return &lumpToken;
}
unsigned char* lumpFind__FPvPc(void* lump, const char* name) {
    if (lump != &lumpToken) events += "badlump;";
    events += std::string("find ") + name + ";";
    static const char* names[] = {"noarrows.tex", "nohealth.tex", "nomana.tex", "nocamera.tex", "countarrows.tex",
                                  "counthealth.tex", "countmana.tex", "icslam.tex", "icfroststorm.tex", "icroot.tex"};
    for (int i = 0; i < 10; ++i) {
        if (std::strcmp(names[i], name) == 0) {
            unsigned v = 1000 + i;
            std::memcpy(entries[i] + 4, &v, 4);
            return entries[i];
        }
    }
    events += "unknown;";
    return entries[0];
}
void HUDInit__Fv() { events += "hud;"; }
}

int main() {
    std::memset(globalIcons, 0xee, sizeof globalIcons);
    gameInitPostFrontend__Fv();
    if (events != "load icons.lmp;find noarrows.tex;find nohealth.tex;find nomana.tex;find nocamera.tex;"
                  "find countarrows.tex;find counthealth.tex;find countmana.tex;find icslam.tex;"
                  "find icfroststorm.tex;find icroot.tex;hud;")
        return 1;
    const int offsets[] = {120, 124, 128, 132, 144, 136, 140, 148, 112, 100};
    unsigned char expect[152];
    std::memset(expect, 0xee, sizeof expect);
    for (int i = 0; i < 10; ++i) {
        unsigned v = 1000 + i;
        std::memcpy(expect + offsets[i], &v, 4);
    }
    if (std::memcmp(expect, globalIcons, sizeof expect) != 0) return 2;
    std::puts("game init post frontend passed");
    return 0;
}

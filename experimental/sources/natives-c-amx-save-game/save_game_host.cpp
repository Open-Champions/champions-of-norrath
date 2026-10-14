#include <cstdio>

extern "C" int amxSaveGame__FP5__amxPi(void*, int*);

static int asked;
static int saved;
static int name_calls;

extern "C" int netGetLocalPlayerNm__Fi(int player) {
    asked = player;
    name_calls += 1;
    return 6;
}

extern "C" void MC_SaveGameBrowser__Fi(int player) { saved = player; }

int main() {
    int params[1] = {0};
    if (amxSaveGame__FP5__amxPi(nullptr, params) != 0) return 1;
    if (name_calls != 1 || asked != 0) return 2;
    if (saved != 6) return 3;
    std::printf("save game passed\n");
    return 0;
}

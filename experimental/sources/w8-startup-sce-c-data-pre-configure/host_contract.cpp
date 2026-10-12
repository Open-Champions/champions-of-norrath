#include <cstdio>
#include <cstring>
#include "pre_configure.cpp"
int main() {
    static_assert(sizeof(preConfigure) == (sizeof(void*) == 4 ? 80 : 120), "five records");
    const char* expected[4] = {"menu_playerone", "menu_playertwo", "menu_playerthree", "menu_playerfour"};
    for (int index = 0; index < 4; ++index) {
        if (preConfigure[index].name == nullptr || std::strcmp(preConfigure[index].name, expected[index]) != 0) return 1;
        for (unsigned word : preConfigure[index].words) if (word != 0) return 2;
    }
    if (preConfigure[4].name != nullptr || preConfigure[4].words[0] != 0 || preConfigure[4].words[2] != 0) return 3;
    std::puts("preConfigure contract passed");
    return 0;
}

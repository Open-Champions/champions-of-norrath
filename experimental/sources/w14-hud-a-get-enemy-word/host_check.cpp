#include <cstdio>
#include <cstring>
static const char* seen = nullptr;
extern "C" const char* textGetString__FPc(const char* key) { seen = key; return key + 1; }
#include "get_enemy_word.cpp"

int main() {
    int bad = 0;
    const int one[] = {1};
    const int other[] = {0, 2, -1, 100};
    for (int v : one) {
        const char* r = getEnemyWord__Fi(v);
        if (!seen || std::strcmp(seen, "enemy") != 0 || r != seen + 1) bad++;
    }
    for (int v : other) {
        const char* r = getEnemyWord__Fi(v);
        if (!seen || std::strcmp(seen, "enemies") != 0 || r != seen + 1) bad++;
    }
    if (!bad) std::puts("getEnemyWord passed");
    return bad;
}

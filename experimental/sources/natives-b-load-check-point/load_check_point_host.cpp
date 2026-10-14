#include <cstdio>
extern "C" int amxLoadCheckPoint__FP5__amxPi(void*, int*);
static int seen = -1;
static int calls = 0;
extern "C" int gameDeathStart__Fi(int value) {
    seen = value;
    calls += 1;
    return 5;
}

int main() {
    if (amxLoadCheckPoint__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    if (calls != 1 || seen != 0) return 2;
    std::puts("amxLoadCheckPoint passed");
    return 0;
}

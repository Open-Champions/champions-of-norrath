#include <cstdio>
extern "C" int amxPlayerSelRunning__FP5__amxPi(void*, int*);
static int calls = 0;
extern "C" int playerSelectionRunning__Fv() {
    calls += 1;
    return 4;
}

int main() {
    if (amxPlayerSelRunning__FP5__amxPi(nullptr, nullptr) != 4) return 1;
    if (calls != 1) return 2;
    std::puts("amxPlayerSelRunning passed");
    return 0;
}

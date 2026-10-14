#include "exit_to_menu.cpp"
#include <cstdio>
static_assert(sizeof(exitToMenuMode) == 1, "exitToMenuMode is one byte");
int main() {
    if (exitToMenuMode != 0) return 1;
    exitToMenuMode = 2;
    if (exitToMenuMode != 2) return 2;
    std::puts("exitToMenuMode contract passed");
    return 0;
}

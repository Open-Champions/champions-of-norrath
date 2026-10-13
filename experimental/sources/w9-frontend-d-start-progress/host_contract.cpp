#include "start_progress.cpp"

#include <cstdio>

int progress;

int main() {
    progress = 77;
    startProgress__Fv();
    if (progress != 0) { std::puts("FAIL progress not cleared"); return 1; }
    std::puts("ok");
    return 0;
}

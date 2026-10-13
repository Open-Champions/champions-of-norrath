// Host check for the line table storage. Not an EE plan input.
#include <cstdio>
#include "game_line_data.cpp"
int main() {
    if (gameLineCount != 0) { std::fprintf(stderr, "fail: count\n"); return 1; }
    for (void* slot : gameLines) if (slot != nullptr) { std::fprintf(stderr, "fail: slot not null\n"); return 2; }
    if (sizeof(gameLines) / sizeof(gameLines[0]) != 100) { std::fprintf(stderr, "fail: slot count\n"); return 3; }
    if (sizeof gameLineCount != 4) { std::fprintf(stderr, "fail: count width\n"); return 4; }
    std::printf("game line storage check passed\n");
    return 0;
}

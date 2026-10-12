// Host contract for gameLineClear. Not an EE plan input.
#include "game_line_clear.cpp"

extern "C" {
int gameLineCount = 0;
void* gameLines[16];
}

static void* destroyed[32];
static int destroyedFlags[32];
static int destroyedCount = 0;
static int shrinkAt = -1, shrinkTo = 0;
extern "C" void destroyLine(void* line, int flags) {
    destroyed[destroyedCount] = line;
    destroyedFlags[destroyedCount] = flags;
    ++destroyedCount;
    if (destroyedCount == shrinkAt) gameLineCount = shrinkTo;
}

int main() {
    int tags[16];
    // Count of zero or negative: no calls, count still zero.
    gameLineCount = 0;
    gameLineClear__Fv();
    if (destroyedCount != 0 || gameLineCount != 0) return 1;
    gameLineCount = -4;
    gameLines[0] = &tags[0];
    gameLineClear__Fv();
    if (destroyedCount != 0 || gameLineCount != 0) return 2;
    // Null entries are skipped, later entries are visited in order, flag is 3.
    gameLines[0] = &tags[0];
    gameLines[1] = 0;
    gameLines[2] = &tags[2];
    gameLines[3] = &tags[3];
    gameLines[4] = &tags[4];  // beyond the count: untouched
    gameLineCount = 4;
    gameLineClear__Fv();
    if (destroyedCount != 3 || destroyed[0] != &tags[0] || destroyed[1] != &tags[2] ||
        destroyed[2] != &tags[3])
        return 3;
    for (int i = 0; i < 3; ++i)
        if (destroyedFlags[i] != 3) return 4;
    if (gameLineCount != 0) return 5;
    // The loop bound is reread after a destructor call that lowers the count.
    destroyedCount = 0;
    gameLines[0] = &tags[0];
    gameLines[1] = &tags[1];
    gameLines[2] = &tags[2];
    gameLineCount = 3;
    shrinkAt = 1;
    shrinkTo = 1;
    gameLineClear__Fv();
    if (destroyedCount != 1 || gameLineCount != 0) return 6;
    return 0;
}

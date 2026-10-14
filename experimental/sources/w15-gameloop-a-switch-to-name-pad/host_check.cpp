// Host check for switchToNamePad__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "switch_to_name_pad.cpp"

#include <cstdio>

unsigned char fontGrid[32];
int nameGridPos;
unsigned char showNoNamePrompt;
float otherHermiteTime;
static int calls, seenX = 9, seenY = 9;
void setNewGridPosition__Fii(int x, int y) { ++calls; seenX = x; seenY = y; }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    for (int i = 0; i < 32; ++i) fontGrid[i] = '#';
    showNoNamePrompt = 1; otherHermiteTime = 5.0f;
    switchToNamePad__Fv();
    for (int i = 0; i < 26; ++i) CHECK(fontGrid[i] == 'a' + i);
    for (int i = 26; i < 32; ++i) CHECK(fontGrid[i] == '#');
    CHECK(nameGridPos == -3 && calls == 1 && seenX == 0 && seenY == 0);
    CHECK(showNoNamePrompt == 0 && otherHermiteTime == 1.0f);
    std::puts("ok");
    return 0;
}

#include "misc_words.cpp"

#include <cstdio>

static int failures;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(sizeof menuModeRebootQuestion == 1 && menuModeRebootQuestion == 0);
    CHECK(title == nullptr);
    CHECK(ranBrowser == 0 && ranConfig == 0);
    if (failures == 0) std::printf("misc_words ok\n");
    return failures != 0;
}

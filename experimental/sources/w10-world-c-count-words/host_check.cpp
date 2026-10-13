// Host contract for the w10-world-c-count-words storage. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "count_words.cpp"

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

int main() {
    expect(nmActors == 0 && nmAlwaysPages == 0 && nmChars == 0 && nmHalos == 0 && nmWaterPatches == 0, "all counters start at zero");
    expect(sizeof(nmActors) == 4 && sizeof(nmHalos) == 4, "word sizes");
    if (failures) return 1;
    std::printf("w10-world-c-count-words contract passed\n");
    return 0;
}

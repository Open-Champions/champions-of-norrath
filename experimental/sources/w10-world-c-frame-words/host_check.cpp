// Host contract for the w10-world-c-frame-words storage. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "frame_words.cpp"

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

int main() {
    expect(noAdvanceTime == 0 && routeClearStart == 0, "zero words");
    expect(resourceFrame == 1, "resourceFrame starts at 1");
    if (failures) return 1;
    std::printf("w10-world-c-frame-words contract passed\n");
    return 0;
}

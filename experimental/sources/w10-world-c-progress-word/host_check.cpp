// Host contract for the w10-world-c-progress-word storage. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "progress_word.cpp"

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

int main() {
    expect(progress == 0.0f && sizeof(progress) == 4, "starts at 0.0, one word");
    if (failures) return 1;
    std::printf("w10-world-c-progress-word contract passed\n");
    return 0;
}

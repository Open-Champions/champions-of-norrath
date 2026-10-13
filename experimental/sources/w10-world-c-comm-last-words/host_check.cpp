// Host contract for the w10-world-c-comm-last-words storage. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "comm_last_words.cpp"

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

int main() {
    expect(lastlastCE == 0 && sizeof(lastlastCE) == 4, "starts at zero, one word");
    if (failures) return 1;
    std::printf("w10-world-c-comm-last-words contract passed\n");
    return 0;
}

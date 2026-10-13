// Host contract for isRevealed__3mapv. Not an EE plan input.
#include "is_revealed.cpp"

#include <cstdio>

int _3map$revealed;

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    const int values[] = {0, 1, 2, -1, 0x12345678};
    for (int v : values) {
        _3map$revealed = v;
        expect(isRevealed__3mapv() == v, "returns the word unchanged");
        expect(_3map$revealed == v, "nothing is stored");
    }
    return failures ? 1 : 0;
}

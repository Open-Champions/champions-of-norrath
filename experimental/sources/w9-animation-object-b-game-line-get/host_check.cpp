// Host contract for gameLineGet__FPc. Not an EE plan input.
#include <cctype>
#include <cstdio>
#include <cstring>
struct LineName { const char* name; };
extern "C" {
LineName* gameLines[8];
int gameLineCount;
int gameLineGet__FPc(char*);
}
static int calls;
extern "C" int strcasecmp(const char* a, const char* b) {
    ++calls;
    for (;; ++a, ++b) {
        int x = std::tolower((unsigned char)*a), y = std::tolower((unsigned char)*b);
        if (x != y) return x - y;
        if (!x) return 0;
    }
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    LineName lines[4] = {{"Alpha"}, {"Beta"}, {"gamma"}, {"beta"}};
    for (int i = 0; i < 4; ++i) gameLines[i] = &lines[i];
    char name[] = "BETA";
    gameLineCount = 4;
    expect(gameLineGet__FPc(name) == 1 && calls == 2, "first match wins and the walk stops there");
    char gamma[] = "GAMMA";
    expect(gameLineGet__FPc(gamma) == 2, "case-insensitive match");
    char missing[] = "delta";
    calls = 0;
    expect(gameLineGet__FPc(missing) == -1 && calls == 4, "no match gives -1 after all comparisons");
    gameLineCount = 2;
    expect(gameLineGet__FPc(gamma) == -1, "entries past the count are not searched");
    gameLineCount = 0;
    calls = 0;
    expect(gameLineGet__FPc(name) == -1 && calls == 0, "empty table");
    gameLineCount = -2;
    expect(gameLineGet__FPc(name) == -1 && calls == 0, "negative count");
    if (failures) return 1;
    std::printf("gameLineGet contract passed\n");
    return 0;
}

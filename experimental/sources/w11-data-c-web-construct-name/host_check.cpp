// Host contract for norrath_web_construct. Not an EE plan input.
#include "web_construct_name.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static int word(const unsigned char* p, int off) { int v; std::memcpy(&v, p + off, 4); return v; }

int main() {
    unsigned char web[96];
    std::memset(web, 0xa5, sizeof web);
    unsigned char want[96];
    std::memcpy(want, web, sizeof want);
    norrath_web_construct(web, 41, 240);
    expect(word(web, 48) == 41 && word(web, 52) == 240, "arguments stored");
    expect(word(web, 8) == 0 && word(web, 28) == 0 && word(web, 44) == 0, "three words cleared");
    const int touched[] = {8, 28, 44, 48, 52};
    for (int off : touched) std::memcpy(want + off, web + off, 4);
    expect(std::memcmp(want, web, sizeof want) == 0, "no other byte changes");
    norrath_web_construct(web, -1, 7);
    expect(word(web, 48) == -1 && word(web, 52) == 7, "other arguments");
    return failures ? 1 : 0;
}

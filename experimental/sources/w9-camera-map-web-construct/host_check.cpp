// Host contract for __3Webii. Not an EE plan input.
#include "web_construct.cpp"

#include <cstdio>
#include <cstring>

extern "C" void norrath_web_construct(void* web, int first, int second);

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static int word(const unsigned char* p, int off) { int v; std::memcpy(&v, p + off, 4); return v; }

int main() {
    for (int use_alias = 0; use_alias < 2; ++use_alias) {
        unsigned char web[96];
        std::memset(web, 0xa5, sizeof web);
        unsigned char before[96];
        std::memcpy(before, web, sizeof web);
        if (use_alias) norrath_web_construct(web, 41, 240);
        else __3Webii(web, 41, 240);
        expect(word(web, 48) == 41 && word(web, 52) == 240, "arguments stored");
        expect(word(web, 8) == 0 && word(web, 28) == 0 && word(web, 44) == 0, "three words cleared");
        unsigned char want[96];
        std::memcpy(want, before, sizeof want);
        const int cleared[] = {8, 28, 44, 48, 52};
        for (int off : cleared) std::memcpy(want + off, web + off, 4);
        expect(std::memcmp(want, web, sizeof want) == 0, "no other byte changes");
        __3Webii(web, -1, 7);
        expect(word(web, 48) == -1 && word(web, 52) == 7, "other arguments");
    }
    return failures ? 1 : 0;
}

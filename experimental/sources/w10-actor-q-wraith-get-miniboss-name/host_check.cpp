// Host check only; not an EE build input.
#include "get_miniboss_name.cpp"
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <string_view>
static int failures;
extern "C" int sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f);
    seen_dest = d;
    int n = std::vsnprintf(d, 16, f, ap);
    va_end(ap);
    return n;
}
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[80] = {0};
    object[76] = 7;
    char* r = getMinibossName__6Wraith(object);
    expect(r == seen_dest, "returns the formatted buffer");
    expect(std::strcmp(r, "MBWraith7") == 0, "variant 7");
    object[76] = 20;
    char* r2 = getMinibossName__6Wraith(object);
    expect(r2 == r, "same static buffer every call");
    expect(std::strcmp(r2, "MBWraith20") == 0, "variant 20");
    object[76] = 0xff;
    expect(std::strcmp(getMinibossName__6Wraith(object), "MBWraith-1") == 0, "byte is signed");
    if (!failures) std::puts("getMinibossName ok");
    return failures ? 1 : 0;
}

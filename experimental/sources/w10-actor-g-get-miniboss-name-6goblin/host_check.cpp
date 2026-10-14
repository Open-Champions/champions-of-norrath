// Host contract for getMinibossName__6Goblin; not an EE input.
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
static std::string fmtSeen; static int valueSeen; static char* destSeen;
extern "C" int sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f); int v = va_arg(ap, int); va_end(ap);
    fmtSeen = f; valueSeen = v; destSeen = d; std::strcpy(d, "stub"); return 4;
}
#include "get_miniboss_name.cpp"
int main() {
    alignas(16) unsigned char object[128]; std::memset(object, 0, sizeof object);
    int bad = 0;
    object[76] = 0xfe;
    char* r = getMinibossName__6Goblin(object);
    if (fmtSeen != "MBGoblin%d") { std::puts("fail: template text"); bad = 1; }
    if (valueSeen != -2) { std::puts("fail: byte at +76 passed sign-extended"); bad = 1; }
    if (r != destSeen) { std::puts("fail: returns the destination buffer"); bad = 1; }
    char* r2 = getMinibossName__6Goblin(object);
    if (r2 != r) { std::puts("fail: same buffer on every call"); bad = 1; }
    if (!bad) std::puts("goblin getMinibossName ok");
    return bad;
}

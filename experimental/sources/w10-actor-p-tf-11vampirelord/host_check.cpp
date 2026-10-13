// Host contract for __tf11VampireLord; not an EE input. Includes the source.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8Creature[12];
static std::string calls;
extern "C" {
void* __tf8Creature() { calls += "tf8Creature;"; return __ti8Creature; }
void* __rtti_si(void* t, const char* n, const void* b) {
    calls += std::string("si:") + (t == __ti11VampireLord ? "own" : "bad") + ":" + n + ":" + (b == __ti8Creature ? "base" : "bad") + ";";
    return t;
}
}
int main() {
    for (unsigned first : {0u, 1u, 0x1234u, 0xffffffffu}) {
        std::memcpy(__ti11VampireLord, &first, 4); calls.clear();
        void* r = __tf11VampireLord();
        std::string want = first == 0 ? "tf8Creature;si:own:11VampireLord:base;" : "";
        if (r != __ti11VampireLord || calls != want) { std::printf("fail first=%u got %s\n", first, calls.c_str()); return 1; }
    }
    return 0;
}

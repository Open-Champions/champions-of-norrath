// Host contract for getMinibossName__6Wraith; not an EE input.
#include "get_miniboss_name.cpp"
#include <cstdio>
#include <cstring>
#include <string>
int main() {
    int failures = 0;
    unsigned char object[128];
    for (int v = -128; v < 128; ++v) {
        std::memset(object, 0xa5, sizeof object);
        object[76] = static_cast<unsigned char>(v);
        const char* name = getMinibossName__6Wraith(object);
        const std::string want = "MBWraith" + std::to_string(v);
        if (want != name) { std::fprintf(stderr, "variant %d gave %s\n", v, name); ++failures; }
        if (name != getMinibossName__6Wraith(object)) { std::fprintf(stderr, "buffer not stable\n"); ++failures; }
        if (want.size() + 1 > 16) { std::fprintf(stderr, "buffer too small\n"); ++failures; }
    }
    if (!failures) std::puts("getMinibossName ok");
    return failures != 0;
}

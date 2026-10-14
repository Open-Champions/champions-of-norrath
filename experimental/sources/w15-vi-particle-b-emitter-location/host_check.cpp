// Host check only; not a build input.
#include "emitter_location.cpp"
#include <cstdio>
#include <cstring>
int main() {
    alignas(16) unsigned char object[2048];
    std::memset(object, 0x5a, sizeof object);
    if (Location__C17VIParticleEmitter(object) != object + 16) { std::puts("FAIL"); return 1; }
    unsigned char* other = object + 8;
    if (Location__C17VIParticleEmitter(other) != other + 16) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}

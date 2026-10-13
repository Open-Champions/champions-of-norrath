#include "delta_time.cpp"

int main() {
    alignas(4) unsigned char system[64] = {};
    *reinterpret_cast<float*>(system + 16) = 0.0625f;
    *reinterpret_cast<float*>(system + 12) = 9.0f;
    *reinterpret_cast<float*>(system + 20) = 8.0f;
    if (DeltaTime__C16VIParticleSystem(system) != 0.0625f) return 1;
    *reinterpret_cast<unsigned*>(system + 16) = 0x80000000u;
    unsigned bits;
    float r = DeltaTime__C16VIParticleSystem(system);
    __builtin_memcpy(&bits, &r, 4);
    return bits == 0x80000000u ? 0 : 2;
}

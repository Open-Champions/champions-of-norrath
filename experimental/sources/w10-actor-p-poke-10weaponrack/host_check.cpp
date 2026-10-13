// Host contract for msg_poke__10WeaponRacki; not an EE input. Includes the source.
#include "poke.cpp"
#include <cstring>

int main() {
    alignas(16) unsigned char object[128];
    unsigned char before[128];
    for (int i = 0; i < 128; ++i) object[i] = static_cast<unsigned char>(i * 5 + 1);
    std::memcpy(before, object, sizeof object);
    msg_poke__10WeaponRacki(object, 3);
    return std::memcmp(before, object, sizeof object) != 0;
}

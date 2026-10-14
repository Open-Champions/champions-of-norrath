// Host contract for SpellEffect__19VISpellEffectSystemi; not an EE input. The authored function was also compared with the
// original in a private R5900 run.
#include "spell_effect.cpp"
#include <cstring>
int main() {
    alignas(16) static unsigned char system[1200];
    alignas(16) static unsigned char block[12 * 5];
    void* const target = &block[0];
    std::memcpy(system + 1120, &(unsigned char* const&)(unsigned char*)block, sizeof(unsigned char*));
    for (int i = 0; i < 5; ++i) {
        void* value = system + i;
        std::memcpy(block + 12 * i, &value, sizeof value);
    }
    (void)target;
    for (int i = 0; i < 5; ++i)
        if (SpellEffect__19VISpellEffectSystemi(system, i) != system + i) return 1 + i;
    return 0;
}

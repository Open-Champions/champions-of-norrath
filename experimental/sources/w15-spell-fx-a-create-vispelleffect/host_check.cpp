// Host contract for Create__13VISpellEffectP7VIScene; not an EE input. The authored function was also compared with the
// original in a private R5900 run.
#include "create.cpp"
#include <cstring>
int main() {
    unsigned char object[32];
    std::memset(object, 0x5a, sizeof object);
    unsigned char before[32];
    std::memcpy(before, object, sizeof object);
    int scene = 0;
    if (Create__13VISpellEffectP7VIScene(object, &scene) != 0) return 1;
    void* stored;
    std::memcpy(&stored, object + 8, sizeof stored);
    if (stored != &scene) return 2;
    std::memcpy(object + 8, before + 8, sizeof stored);
    return std::memcmp(object, before, sizeof object) == 0 ? 0 : 3;
}

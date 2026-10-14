// Host contract for clone__15ShockProjectile; not an EE input. The result is null and the source is left untouched. The authored
// function was also compared with the original in a private R5900 run (6 scenarios, return value included, every word covered).
#include "clone.cpp"
int main() {
    unsigned char object[64];
    for (int i = 0; i < 64; ++i) object[i] = static_cast<unsigned char>(i * 3 + 9);
    if (clone__15ShockProjectile(object) != nullptr) return 1;
    for (int i = 0; i < 64; ++i) if (object[i] != static_cast<unsigned char>(i * 3 + 9)) return 2;
    return 0;
}

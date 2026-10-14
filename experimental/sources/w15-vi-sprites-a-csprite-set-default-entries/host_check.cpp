// Host check only; not a build input.
#include "set_default_entries.cpp"
#include <cstring>
#include <cstdlib>
static void entry(unsigned char* o, int i, int* w) { std::memcpy(w, o + 424 + 52 * i, 52); }
int main() {
    // reference model written from the description in the source header
    static const int copyFrom23[] = {23, 25, 26, 28, 29, 31, 33, 35, 41, 42, 43, 44, 50, 51, 52, 53};
    static const int fromD[] = {22, 24, 27, 30, 32, 36, 40};
    static const int skip[] = {19, 20, 21, 47, 48, 49, 54, 55, 58, 62};
    static const int flag1[] = {7, 56, 57, 59, 60, 61, 63, 64};
    auto in = [](const int* a, int n, int v) { for (int k = 0; k < n; ++k) if (a[k] == v) return true; return false; };
    alignas(8) static unsigned char o[4096], m[4096];
    std::srand(5);
    for (int trial = 0; trial < 400; ++trial) {
        for (int k = 0; k < 4096; ++k) o[k] = static_cast<unsigned char>(std::rand());
        int race = (trial & 1) ? 9 : 3; std::memcpy(o + 408, &race, 4);
        for (int i = 0; i < 66; ++i) {
            int v = (std::rand() % 3 == 0 || (trial % 8 > 5 && i >= 22)) ? -1 : (std::rand() & 0x3fffffff) + 1;
            std::memcpy(o + 424 + 52 * i, &v, 4);
        }
        float scale = 3.5f; std::memcpy(o + 424 + 52 * 41 + 8, &scale, 4);
        std::memcpy(m, o, sizeof o);
        auto w0 = [&](int i) { int v; std::memcpy(&v, m + 424 + 52 * i, 4); return v; };
        for (int i = 0; i < 66; ++i) {
            if (w0(i) != -1) continue;
            int src = -1;
            if (in(skip, 10, i)) continue;
            if (i == 65) {
                int e41 = w0(41);
                if (race == 9) src = 0; else if (e41 != -1) src = 41; else src = 0;
                std::memmove(m + 424 + 52 * i, m + 424 + 52 * src, 52);
                int w[13]; entry(m, i, w);
                if (race == 9 || e41 == -1) w[3] = 1; else { float f; std::memcpy(&f, &w[2], 4); f *= 0.5f; std::memcpy(&w[2], &f, 4); }
                w[6] = -1; w[7] = -1; std::memcpy(m + 424 + 52 * i, w, 52);
                continue;
            }
            if (in(copyFrom23, 16, i)) src = w0(23) != -1 ? 23 : 0;
            else if (in(fromD, 7, i)) src = w0(22) != -1 ? 22 : w0(23) != -1 ? 23 : 0;
            else src = 0;
            std::memmove(m + 424 + 52 * i, m + 424 + 52 * src, 52);
            if (in(flag1, 8, i)) { int one = 1; std::memcpy(m + 424 + 52 * i + 12, &one, 4); }
        }
        if (SetDefaultEntries__9VICSprite(o) != 0) return 1;
        if (std::memcmp(o, m, sizeof o)) return 2;
    }
    return 0;
}

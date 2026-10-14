// Host check only; not a build input.
#include <cstring>
static int n, log_[64], seen_pal[64], seen_entry[64], seen_layer[64]; static unsigned seen_mat[64];
extern "C" unsigned GetMaterialPalEntry__8VIRasterii(unsigned char*, int p, int e) { log_[n] = 1; seen_pal[n] = p; seen_entry[n++] = e; return 1000 + e; }
extern "C" unsigned GetLayerTexture__10VIMateriali(unsigned m, int l) { log_[n] = 2; seen_mat[n] = m; seen_layer[n++] = l; return m * 10 + l; }
#include "init_text_slots.cpp"
#include <cstdio>
static int& I(unsigned char* s, int off) { return *reinterpret_cast<int*>(s + off); }
int main() {
    alignas(8) unsigned char s[4400];
    std::memset(s, 0xff, sizeof s);
    I(s, 416) = 77;
    for (int k = 0; k < 19; ++k) { I(s, 3900 + 24 * k + 4) = k; I(s, 3900 + 24 * k + 8) = 0x1234; }
    I(s, 3900 + 24 * 3) = 5; I(s, 3900 + 24 * 18) = 6; I(s, 3900 + 24 * 0) = 0;
    n = 0;
    bool ok = InitTextSlots__9VICSpriteR8VIRaster(s, nullptr) == 0 && n == 6;
    ok = ok && log_[0] == 1 && seen_pal[0] == 77 && seen_entry[0] == 0 && log_[1] == 2 && seen_mat[1] == 1000 && seen_layer[1] == 0
        && seen_entry[2] == 5 && seen_layer[3] == 3 && seen_entry[4] == 6 && seen_layer[5] == 18;
    ok = ok && I(s, 3900 + 8) == 10000 && I(s, 3900 + 24 * 3 + 8) == 10053 && I(s, 3900 + 24 * 18 + 8) == 10078;
    ok = ok && I(s, 3900 + 24 + 8) == 0x1234 && I(s, 3900 + 24 * 19 + 8) == -1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

// Host check only; not a build input.
#include "deallocate.cpp"
#include <cstdio>
static unsigned freed, calls;
extern "C" void __builtin_vec_delete(unsigned b) { freed = b; ++calls; }
int main() {
    unsigned s[96];
    for (int i = 0; i < 96; ++i) s[i] = 0x01010101u * (i + 1);
    unsigned keep[96]; for (int i = 0; i < 96; ++i) keep[i] = s[i];
    s[78] = 0;
    bool ok = Deallocate__9VISurface(reinterpret_cast<unsigned char*>(s)) == 0 && calls == 0;
    for (int i = 0; i < 96; ++i) ok = ok && (i == 78 || s[i] == keep[i]);
    s[78] = 0x1234;
    ok = ok && Deallocate__9VISurface(reinterpret_cast<unsigned char*>(s)) == 0 && calls == 1 && freed == 0x1234;
    const int idx[] = {2, 3, 4, 5, 6, 7, 8, 16, 77, 78};
    // offsets 8,12,16,20,24,28,32,64,308,312 -> words 2,3,4,5,6,7,8,16,77,78
    const unsigned expect[] = {0, 0, 2, 0xffffffffu, 0xffffffffu, 0xffffffffu, 1, 0, 0, 0};
    for (int k = 0; k < 10; ++k) ok = ok && s[idx[k]] == expect[k];
    for (int i = 0; i < 96; ++i) { bool touched = false; for (int k = 0; k < 10; ++k) touched |= idx[k] == i; if (!touched) ok = ok && (i == 78 || s[i] == keep[i]); }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

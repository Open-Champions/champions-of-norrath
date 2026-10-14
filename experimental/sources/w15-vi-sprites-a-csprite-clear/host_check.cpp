// Host check only; not a build input.
#include "clear.cpp"
#include <cstring>
#include <initializer_list>
static int calls, ret; static void* seen;
extern "C" int Clear__9VIHSprite(void* o) { ++calls; seen = o; return ret; }
int main() {
    alignas(8) static unsigned char s[4608], e[4608];
    for (int r : {0, -4}) {
        std::memset(s, 0x5a, sizeof s); std::memset(e, 0x5a, sizeof e); calls = 0; ret = r; seen = 0;
        int v = Clear__9VICSprite(s);
        if (calls != 1 || seen != s || v != (r < 0 ? -1 : 0)) return 1;
        if (r < 0) { if (std::memcmp(s, e, sizeof s)) return 2; continue; }
        auto w = [&](int off, int x) { std::memcpy(e + off, &x, 4); };
        auto f = [&](int off, float x) { std::memcpy(e + off, &x, 4); };
        w(400, 1); w(52, 5); w(416, -1); w(420, -1); w(3856, -1); w(4456, 0); w(4588, 0); w(4584, -1);
        for (int b : {4460, 4476, 4492, 4508}) { w(b, 0); w(b + 4, -1); w(b + 8, 0); w(b + 12, -1); }
        for (int off = 4528; off <= 4544; off += 4) w(off, 0);
        f(4548, 0.05f); f(4552, 0.05f); f(4556, 0.3f); f(4560, 1.0f);
        for (int off = 4568; off <= 4576; off += 4) w(off, 0);
        w(4580, -1);
        for (int g = 0; g < 3; ++g) { w(4364 + 12 * g, -1); w(4368 + 12 * g, 11); w(4372 + 12 * g, -1); }
        for (int off = 3860; off <= 3892; off += 4) w(off, -1);
        for (int off = 4400; off <= 4420; off += 4) w(off, -1);
        if (std::memcmp(s, e, sizeof s)) return 3;
    }
    return 0;
}

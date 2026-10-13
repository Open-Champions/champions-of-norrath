// Host check only; not a build input.
#include "getcaretpos.cpp"
#include <cstdio>
#include <cstring>

int main() {
    bool ok = true;
    // Two rows: lengths 5 and 3; the first ends in a line break (flag set).
    static unsigned char rows[16];
    const short r0[2] = {5, 1}, r1[2] = {3, 0};
    std::memcpy(rows + 4, r0, 4);
    std::memcpy(rows + 12, r1, 4);
    if (reinterpret_cast<unsigned long>(rows) > 0xffffffffUL) { std::puts("skip (64-bit host address)"); return 0; }
    struct Edit { unsigned char head[404]; int caret; unsigned char pad[24]; unsigned rows; int count; } e;
    std::memset(&e, 0, sizeof e);
    e.rows = static_cast<unsigned>(reinterpret_cast<unsigned long>(rows));
    e.count = 2;
    const int expect[][3] = {{0, 0, 0}, {3, 3, 0}, {5, 0, 1}, {6, 1, 1}, {8, 3, 1}};
    for (const auto& c : expect) {
        e.caret = c[0];
        int out[2] = {-7, -7};
        ok = ok && GetCaretPos__9VIWndEditP8VIVect2I(&e, out) == 0 && out[0] == c[1] && out[1] == c[2];
    }
    e.caret = 9;
    int out[2] = {-7, -7};
    ok = ok && GetCaretPos__9VIWndEditP8VIVect2I(&e, out) == -1 && out[0] == -7;
    e.count = 0;
    ok = ok && GetCaretPos__9VIWndEditP8VIVect2I(&e, out) == -1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

// Host check only; not a build input.
#include "unlock.cpp"
#include <cstdio>
int main() {
    int s[10] = {};
    s[7] = 3;
    bool ok = UnlockMipLevel__9VISurfacei(s, 4) == -1 && s[7] == 3;   // different level: untouched
    ok = ok && UnlockMipLevel__9VISurfacei(s, 3) == 0 && s[7] == -1;  // same level: cleared
    ok = ok && UnlockMipLevel__9VISurfacei(s, 3) == -1 && s[7] == -1; // already unlocked
    s[7] = 0;
    ok = ok && UnlockMipLevel__9VISurfacei(s, 0) == 0 && s[7] == -1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

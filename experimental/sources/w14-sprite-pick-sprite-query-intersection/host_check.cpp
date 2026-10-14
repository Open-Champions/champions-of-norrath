// Host check only; not a build input.
#include "query.cpp"
int main() {
    alignas(8) unsigned char a[64];
    for (int i = 0; i < 64; ++i) a[i] = static_cast<unsigned char>(i * 5 + 1);
    unsigned char before[64];
    for (int i = 0; i < 64; ++i) before[i] = a[i];
    if (QueryIntersection__8VISpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene(a+0, a+8, a+16, a+24, a+32) != 0) return 1;
    for (int i = 0; i < 64; ++i) if (a[i] != before[i]) return 2;
    return 0;
}

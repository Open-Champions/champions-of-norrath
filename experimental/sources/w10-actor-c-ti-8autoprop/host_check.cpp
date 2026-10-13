#include <cstdio>
extern "C" unsigned char __ti8AutoProp[12];
int main() {
    int failures = 0;
    if (sizeof(__ti8AutoProp) != 12) { std::fprintf(stderr, "fail: size\n"); ++failures; }
    for (unsigned char b : __ti8AutoProp) if (b != 0) { std::fprintf(stderr, "fail: not zero\n"); ++failures; break; }
    if ((reinterpret_cast<unsigned long>(__ti8AutoProp) & 3u) != 0) { std::fprintf(stderr, "fail: alignment\n"); ++failures; }
    return failures ? 1 : 0;
}

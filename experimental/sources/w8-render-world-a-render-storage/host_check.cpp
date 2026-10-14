#include "render_storage.cpp"
#include <cstdio>
int main() {
    int failures = 0;
    if (lastNmBlocksFree != 100000) { std::fprintf(stderr, "fail: lastNmBlocksFree\n"); ++failures; }
    if (cacheHead || cacheTail || lastNmQ || checkBlock || loadSampleRate) { std::fprintf(stderr, "fail: zero words\n"); ++failures; }
    if (sizeof decompressStack != 16384u || sizeof cache != 192u) { std::fprintf(stderr, "fail: extents\n"); ++failures; }
    for (unsigned i = 0; i < sizeof cache; ++i) if (cache[i]) { ++failures; break; }
    return failures;
}

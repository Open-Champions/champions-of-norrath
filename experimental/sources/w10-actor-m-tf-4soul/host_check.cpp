// Host contract for __tf4Soul; not an EE input.
#include <cstring>
#include <cstdio>
static int base_calls, rtti_calls; static void* seen_obj; static const char* seen_name; static const void* seen_base;
extern "C" unsigned char __ti8Creature[12] = {};
#include "typeinfo.cpp"
extern "C" void* __tf8Creature() { ++base_calls; return nullptr; }
extern "C" void* __rtti_si(void* t, const char* n, const void* b) { ++rtti_calls; seen_obj = t; seen_name = n; seen_base = b; static_cast<unsigned char*>(t)[0] = 2; return t; }
int main() {
    int bad = 0;
    std::memset(__ti4Soul, 0, 12);
    void* a = __tf4Soul();
    if (a != __ti4Soul || base_calls != 1 || rtti_calls != 1 || seen_obj != __ti4Soul || seen_base != __ti8Creature || std::strcmp(seen_name, "4Soul")) bad = 1;
    a = __tf4Soul();
    if (a != __ti4Soul || base_calls != 1 || rtti_calls != 1) bad = 1;
    for (int b = 0; b < 4; ++b) {
        base_calls = rtti_calls = 0; std::memset(__ti4Soul, 0, 12); __ti4Soul[b] = 1;
        if (__tf4Soul() != __ti4Soul || base_calls || rtti_calls) bad = 1;
    }
    base_calls = rtti_calls = 0; std::memset(__ti4Soul, 0, 12); __ti4Soul[4] = 9;
    __tf4Soul();
    if (base_calls != 1 || rtti_calls != 1) bad = 1;
    return bad;
}

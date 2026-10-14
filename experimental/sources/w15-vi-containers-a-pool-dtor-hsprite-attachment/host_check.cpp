// Host check only; not a build input. DestroyPool can leave the storage word alone or clear it,
// the two outcomes the original branches on.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static unsigned o[8];
static std::string trace;
static bool clear_word;
extern "C" int DestroyPool__t6VIPool1Z19VIHSpriteAttachment(void* self) {
    trace += self == o ? "destroy;" : "destroy?;";
    if (clear_word) static_cast<unsigned*>(self)[2] = 0;
    return 0;
}
extern "C" void __builtin_vec_delete(void* p) { trace += reinterpret_cast<unsigned long>(p) == 0x1000 ? "vecdel;" : "vecdel?;"; }
extern "C" void __builtin_delete(void* p) { trace += p == o ? "delete;" : "delete?;"; }
int main() {
    bool ok = true;
    const int flag_values[] = {0, 1, 2, 3, -1, -2};
    for (int has = 0; has < 2; ++has)
        for (int clear = 0; clear < 2; ++clear)
            for (int flags : flag_values) {
                trace.clear();
                clear_word = clear;
                std::memset(o, 0x5a, sizeof o);
                o[2] = has ? 0x1000 : 0;
                _$_t6VIPool1Z19VIHSpriteAttachment(o, flags);
                std::string want;
                if (has) { want += "destroy;"; if (!clear) want += "vecdel;"; }
                if (flags & 1) want += "delete;";
                if (trace != want) { std::puts(trace.c_str()); ok = false; }
                for (int i = 0; i < 8; ++i)
                    if (i != 2 && o[i] != 0x5a5a5a5au) ok = false;
            }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

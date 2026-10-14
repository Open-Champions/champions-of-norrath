// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int rel_calls, share_calls, rel_result, share_result, rel_seen_index, share_seen_index;
extern "C" int ReleaseMaterialPal__8VIRasteriR12VIDictionary(unsigned char*, int i, unsigned char*) { ++rel_calls; rel_seen_index = i; return rel_result; }
extern "C" int ShareMaterialPal__8VIRasteri(unsigned char*, int i) { ++share_calls; share_seen_index = i; return share_result; }
#include "set_material_pal.cpp"
#include <cstdio>
#include <initializer_list>
int main() {
    bool ok = true;
    for (int rr : {0, 4, -1}) for (int sr : {0, 4, -1}) {
        *reinterpret_cast<int*>(arena + 72) = 11;
        rel_calls = share_calls = 0; rel_result = rr; share_result = sr;
        int v = SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary(arena, 22, nullptr, nullptr);
        if (rr < 0) ok = ok && v == -1 && share_calls == 0 && *reinterpret_cast<int*>(arena + 72) == 11 && rel_seen_index == 11;
        else ok = ok && v == (sr < 0 ? -1 : 0) && share_calls == 1 && *reinterpret_cast<int*>(arena + 72) == 22 && share_seen_index == 22 && rel_seen_index == 11;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

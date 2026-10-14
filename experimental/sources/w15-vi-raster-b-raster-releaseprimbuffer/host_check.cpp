// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int log_[16], n;
static unsigned char* seen_a[16]; static unsigned char* seen_b[16]; static int seen_i[16]; static unsigned seen_w[16];
static int remove_ret, add_ret, erase_ret;
extern "C" int Remove__12VIDictionaryR10VIResource(unsigned char* d, unsigned char* r) { log_[n] = 1; seen_a[n] = d; seen_b[n++] = r; return remove_ret; }
extern "C" int Add__t6VIList1ZP12VIPrimBufferRCP12VIPrimBuffer(unsigned char* l, const unsigned* e) { log_[n] = 2; seen_a[n] = l; seen_w[n++] = *e; return add_ret; }
extern "C" int Erase__t6VIPool1ZP12VIPrimBufferi(unsigned char* p, int i) { log_[n] = 3; seen_a[n] = p; seen_i[n++] = i; return erase_ret; }
#include "release_prim_buffer.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    unsigned char* dict = arena + 0x9000;
    W(19320 + 8) = arena_base + 0x6000;
    W(17880) = 0xabcd;
    for (unsigned i = 0; i < 3; ++i) W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x40 * i;
    bool ok = true;
    W(0x7044) = 3; W(0x7040) = 9; n = 0;
    ok = ok && ReleasePrimBuffer__8VIRasteriR12VIDictionary(arena, 1, dict) == 0 && W(0x7044) == 2 && n == 0 && W(0x7050) == 0;
    W(0x7044) = 1; remove_ret = 0; add_ret = 0; erase_ret = 0; n = 0;
    ok = ok && ReleasePrimBuffer__8VIRasteriR12VIDictionary(arena, 1, dict) == 0 && W(0x7044) == 0 && n == 3 && W(0x7050) == 0xabcd
        && log_[0] == 1 && seen_a[0] == dict && seen_b[0] == arena + 0x7040
        && log_[1] == 2 && seen_a[1] == arena + 19344 && seen_w[1] == arena_base + 0x7040
        && log_[2] == 3 && seen_a[2] == arena + 19320 && seen_i[2] == 1;
    W(0x7084) = 1; W(0x7080) = 0; n = 0;
    ok = ok && ReleasePrimBuffer__8VIRasteriR12VIDictionary(arena, 2, dict) == 0 && n == 2 && log_[0] == 2 && log_[1] == 3 && seen_i[1] == 2;
    W(0x7004) = 1; W(0x7000) = 4; remove_ret = -1; n = 0; W(0x7010) = 0;
    ok = ok && ReleasePrimBuffer__8VIRasteriR12VIDictionary(arena, 0, dict) == -1 && n == 1 && W(0x7010) == 0;
    W(0x7004) = 1; remove_ret = 0; add_ret = -1; n = 0;
    ok = ok && ReleasePrimBuffer__8VIRasteriR12VIDictionary(arena, 0, dict) == -1 && n == 2 && W(0x7010) == 0xabcd;
    W(0x7004) = 1; add_ret = 0; erase_ret = -1; n = 0;
    ok = ok && ReleasePrimBuffer__8VIRasteriR12VIDictionary(arena, 0, dict) == -1 && n == 3;
    W(0x7004) = 0; erase_ret = 5; n = 0;
    ok = ok && ReleasePrimBuffer__8VIRasteriR12VIDictionary(arena, 0, dict) == 0 && (int)W(0x7004) == -1 && n == 3;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

// Host check only; not a build input. The slot function is a recorder.
#define NORRATH_HOST_ARENA 1
#include "copy_sprite.cpp"
#include <cstdint>
static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static int calls; static unsigned char *g_p, *g_s; static unsigned g_a, g_b, g_c; static int g_v;
static int slot_fn(unsigned char* p, unsigned char* s, unsigned a, unsigned b, unsigned c, int v) { ++calls; g_p = p; g_s = s; g_a = a; g_b = b; g_c = c; g_v = v; return 77; }
extern "C" void* norrath_host_function(unsigned) { return reinterpret_cast<void*>(slot_fn); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
int main() {
    static unsigned char S[0x6000];                     // the scene context words live at +19356
    unsigned char* records = arena + 0x100;
    unsigned char* sprite = arena + 0x400;
    unsigned char* table = arena + 0x800;
    *reinterpret_cast<unsigned*>(S + 1004) = word_of(records);
    *reinterpret_cast<unsigned*>(records + 12 * 3) = word_of(sprite);
    *reinterpret_cast<unsigned*>(sprite + 60) = word_of(table);
    *reinterpret_cast<short*>(table + 72) = -24;
    *reinterpret_cast<unsigned*>(table + 76) = 0x1234;
    *reinterpret_cast<unsigned*>(S + 19356) = 0xa1; *reinterpret_cast<unsigned*>(S + 19360) = 0xb2; *reinterpret_cast<unsigned*>(S + 19364) = 0xc3;
    int r = CopySprite__7VISceneii(S, 3, 9);
    bool ok = r == 77 && calls == 1 && g_p == sprite - 24 && g_s == S && g_a == 0xa1 && g_b == 0xb2 && g_c == 0xc3 && g_v == 9;
    return ok ? 0 : 1;
}

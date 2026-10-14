#define NORRATH_HOST_CONTRACT 1
int norrath_options_text_def __asm__("VIWndOptions$sBattleMusicText") = 99;
#include "init.cpp"

#include <cstring>
#include <vector>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static unsigned off_of(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }

struct Ev { int kind; unsigned a, b; };
static std::vector<Ev> ev;
extern "C" void Init__13VIBattleMusicP7VIScene(void* m, VIScene* s) { ev.push_back({1, off_of(m), (unsigned)(unsigned long)s}); }
extern "C" void RemoveChild__5VIWndP5VIWnd(void* w, void* c) { ev.push_back({2, off_of(w), off_of(c)}); }
extern "C" void Init__12VIWndMessage(void* m) { ev.push_back({3, off_of(m), 0}); }
extern "C" void AddChild__5VIWndP5VIWnd(void* w, void* c) { ev.push_back({4, off_of(w), off_of(c)}); }

int main() {
    const unsigned self = 0x100;
    for (unsigned child : {0u, 0x5000u}) {
        std::memset(arena, 0x5a, sizeof arena);
        ev.clear();
        put(self + 1196, child);
        norrath_options_text_def = 99;
        VIScene* scene = reinterpret_cast<VIScene*>(0x777);
        if (Init__12VIWndOptionsP7VIScene(arena + self, scene) != 0) return 1;
        if (get(self + 1180) != 0 || get(self + 36) != 0 || get(self + 1260) != 0) return 2;
        if (norrath_options_text_def != 0) return 3;
        if (get(self + 1216) != 0x3ca3d70au) return 4;
        if (get(self + 1196) != 0) return 5;
        size_t k = 0;
        if (ev[k].kind != 1 || ev[k].a != self + 1200 || ev[k].b != 0x777) return 6; ++k;
        if (child) { if (ev[k].kind != 2 || ev[k].a != self || ev[k].b != child) return 7; ++k; }
        if (ev[k].kind != 3 || ev[k].a != self + 1232) return 8; ++k;
        if (ev[k].kind != 4 || ev[k].a != self || ev[k].b != self + 1232) return 9; ++k;
        if (k != ev.size()) return 10;
        for (unsigned i = 0; i < 1400; ++i) {
            bool written = (i >= 1180 && i < 1184) || (i >= 36 && i < 40) || (i >= 1196 && i < 1200) ||
                           (i >= 1216 && i < 1220) || (i >= 1260 && i < 1264);
            if (!written && arena[self + i] != 0x5a && !(child && i >= 1196 && i < 1200)) return 11;
        }
    }
    return 0;
}

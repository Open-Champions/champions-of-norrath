#include "create_sprite.cpp"

#include <cstring>
#include <vector>

struct Ev { int kind; long a, b; };
static std::vector<Ev> ev;
static char heap[8][8192];
static int next_heap;
static bool fail_alloc;
static int add_result = 41;
static VISprite* added;
static void* pool_seen;

extern "C" void* __builtin_new(unsigned size) {
    ev.push_back({1, (long)size, 0});
    if (fail_alloc) return nullptr;
    std::memset(heap[next_heap], 0, sizeof heap[0]);
    return heap[next_heap++];
}
extern "C" int Add__t6VIPool1ZP8VISpriteRCP8VISprite(void* pool, VISprite* const* sprite) {
    ev.push_back({3, 0, 0});
    pool_seen = pool;
    added = *sprite;
    return add_result;
}
extern "C" void __14VISimpleSprite(void* s) { ev.push_back({2, 1, (long)s}); }
extern "C" void __13VIPointSprite(void* s) { ev.push_back({2, 2, (long)s}); }
extern "C" void __12VISkinSprite(void* s) { ev.push_back({2, 3, (long)s}); }
extern "C" void __9VIHSprite(void* s) { ev.push_back({2, 4, (long)s}); }
extern "C" void __9VICSprite(void* s) { ev.push_back({2, 5, (long)s}); }
extern "C" void __12VINameSprite(void* s) { ev.push_back({2, 6, (long)s}); }
extern "C" void __11VILODSprite(void* s) { ev.push_back({2, 7, (long)s}); }
extern "C" void __12VIPointLight(void* s) { ev.push_back({2, 8, (long)s}); }
extern "C" void __13VISoundSprite(void* s) { ev.push_back({2, 9, (long)s}); }
extern "C" void __16VIParticleSprite(void* s) { ev.push_back({2, 10, (long)s}); }
extern "C" void __13VIGroupSprite(void* s) { ev.push_back({2, 11, (long)s}); }
extern "C" void __19VIStreamAudioSprite(void* s) { ev.push_back({2, 12, (long)s}); }
extern "C" void __13VIFloraSprite(void* s) { ev.push_back({2, 13, (long)s}); }
extern "C" void __20VIEffectVolumeSprite(void* s) { ev.push_back({2, 14, (long)s}); }

int main() {
    static const unsigned sizes[15] = {0, 76, 68, 76, 400, 4624, 116, 112, 80, 88, 1088, 228, 200, 72, 192};
    alignas(8) static char scene[2048];
    for (int type = 1; type <= 14; ++type) {
        for (int addRef = 0; addRef <= 1; ++addRef) {
            ev.clear(); next_heap = 0; fail_alloc = false; added = nullptr; pool_seen = nullptr;
            int r = CreateSprite__7VIScene12VISpriteTypei(reinterpret_cast<VIScene*>(scene), type, addRef);
            if (r != 41) return 1;
            if (ev.size() != 3) return 2;
            if (ev[0].kind != 1 || ev[0].a != (long)sizes[type]) return 3;
            if (ev[1].kind != 2 || ev[1].a != type || ev[1].b != (long)heap[0]) return 4;
            if (ev[2].kind != 3 || added != (VISprite*)heap[0] || pool_seen != scene + 996) return 5;
            int count; std::memcpy(&count, heap[0] + 4, 4);
            if (count != addRef) return 6;
        }
    }
    // a failed allocation: no constructor, no pool add, -1
    ev.clear(); next_heap = 0; fail_alloc = true;
    if (CreateSprite__7VIScene12VISpriteTypei(reinterpret_cast<VIScene*>(scene), 4, 1) != -1 || ev.size() != 1) return 7;
    // unhandled types: nothing at all happens
    const int bad[] = {0, 15, 16, -1, 100, -2147483647 - 1};
    for (int type : bad) {
        ev.clear(); fail_alloc = false;
        if (CreateSprite__7VIScene12VISpriteTypei(reinterpret_cast<VIScene*>(scene), type, 1) != -1 || !ev.empty()) return 8;
    }
    // the pool result is returned as is
    add_result = -7; next_heap = 0; fail_alloc = false;
    if (CreateSprite__7VIScene12VISpriteTypei(reinterpret_cast<VIScene*>(scene), 2, 0) != -7) return 9;
    return 0;
}

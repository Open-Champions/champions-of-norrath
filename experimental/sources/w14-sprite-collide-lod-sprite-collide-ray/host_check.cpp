// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "collide.cpp"
#include <cstdint>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static int result[2] = {0, 0};
static unsigned char *ga[5]; static int calls, which;
static int hit(int k, unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { ga[0]=a; ga[1]=b; ga[2]=c; ga[3]=d; ga[4]=e; ++calls; which = k; return result[k]; }
static int f0(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { return hit(0,a,b,c,d,e); }
static int f1(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d, unsigned char* e) { return hit(1,a,b,c,d,e); }
extern "C" void* norrath_host_function(unsigned w) { return w == 1 ? reinterpret_cast<void*>(f0) : w == 2 ? reinterpret_cast<void*>(f1) : nullptr; }
static int sceneIndex, sceneCalls; static unsigned spriteWord;
extern "C" unsigned Sprite__7VIScenei(unsigned char* scene, int index) { sceneIndex = index; ++sceneCalls; (void)scene; return spriteWord; }
int main() {
    unsigned char* self = arena + 0x100;
    unsigned char* sprite = arena + 0x400;
    unsigned char* tab = arena + 0x800;
    unsigned char* shape = arena + 0x1000; unsigned char* filt = arena + 0x1010; unsigned char* scene = arena + 0x1020; unsigned char* coll = arena + 0x1030;
    for (int k = 0; k < 2; ++k) for (int adj = -16; adj <= 16; adj += 16) for (int idx = 0; idx < 2; ++idx) {
        for (unsigned i = 0; i < 0x2000; ++i) arena[i] = static_cast<unsigned char>(i * 7 + 3);
        *reinterpret_cast<int*>(self + 76) = idx ? -5 : 1234;
        spriteWord = word_of(sprite);
        *reinterpret_cast<unsigned*>(sprite + 60) = word_of(tab);
        unsigned char* e = tab + 40;
        *reinterpret_cast<short*>(e) = static_cast<short>(adj);
        *reinterpret_cast<unsigned*>(e + 4) = k + 1;
        // the neighbouring entries call the other function so a wrong slot is seen
        unsigned char* o = tab + 40 + ((40==32)?8:-8);
        *reinterpret_cast<unsigned*>(o + 4) = 2 - k;
        result[0] = 11; result[1] = -22; calls = 0; sceneCalls = 0;
        int got = Collide__11VILODSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide(self, shape, filt, scene, coll);
        if (sceneCalls != 1 || sceneIndex != (idx ? -5 : 1234)) return 1;
        if (calls != 1 || which != k) return 2;
        if (got != result[k]) return 3;
        if (ga[0] != sprite + adj || ga[1] != shape || ga[2] != filt || ga[3] != scene || ga[4] != coll) return 4;
    }
    return 0;
}

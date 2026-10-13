// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "release.cpp"
#include <cstdint>
#include <cstring>
alignas(16) static unsigned char arena[0x10000];
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
// Pool iteration stubs follow the observed behaviour of the image's own routines: the first index is the word at pool+16 (-1 means End), the entry
// array is the word at pool+8, entries are 16 (attachment) or 104 (play) bytes and the link to the next entry is the word at +8 (attachment) or +96 (play).
static unsigned char* arr_of(const unsigned char* pool) { return norrath_host_pointer(*reinterpret_cast<const unsigned*>(pool + 8)); }
extern "C" int End__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char*) { return 0x7fffffff; }
extern "C" int End__Ct6VIPool1Z13VIHSpritePlay(const unsigned char*) { return 0x7fffffff; }
extern "C" int Begin__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char* p) { int f = *reinterpret_cast<const int*>(p + 16); return f != -1 ? f : 0x7fffffff; }
extern "C" int Begin__Ct6VIPool1Z13VIHSpritePlay(const unsigned char* p) { int f = *reinterpret_cast<const int*>(p + 16); return f != -1 ? f : 0x7fffffff; }
extern "C" int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(const unsigned char* p, int i) { int n = *reinterpret_cast<int*>(arr_of(p) + 16 * i + 8); return n != -1 ? n : 0x7fffffff; }
extern "C" int Next__Ct6VIPool1Z13VIHSpritePlayi(const unsigned char* p, int i) { int n = *reinterpret_cast<int*>(arr_of(p) + 104 * i + 96); return n != -1 ? n : 0x7fffffff; }
typedef unsigned char* P;
struct Call { int id; intptr_t a, b; };
static Call log_[64]; static int n_; static Call expect_[64]; static int expect_n;
static void rec(int id, intptr_t a, intptr_t b = 0) { log_[n_++] = Call{id, a, b}; }
static void ex(int id, intptr_t a, intptr_t b = 0) { expect_[expect_n++] = Call{id, a, b}; }
static bool same() { if (n_ != expect_n) return false; for (int i = 0; i < n_; ++i) if (log_[i].id != expect_[i].id || log_[i].a != expect_[i].a || log_[i].b != expect_[i].b) return false; return true; }
static int removeRet, failAt, calls;
static P scene_, dict_;
extern "C" int Remove__12VIDictionaryR10VIResource(P d, P r) { rec(1, (intptr_t)d, (intptr_t)r); return removeRet; }
static int tick(int id, int arg) { rec(id, (intptr_t)scene_, arg); return (++calls == failAt) ? -4 : 3; }
extern "C" int ReleaseSprite__7VIScenei(P, int s) { return tick(2, s); }
extern "C" int ReleaseAnimation__7VIScenei(P, int a) { return tick(3, a); }
extern "C" int ReleaseRefMap__7VIScenei(P, int m) { return tick(4, m); }
int main() {
    unsigned char* self = arena + 0x100; scene_ = arena + 0x2000; dict_ = arena + 0x2010;
    unsigned char* arr1 = arena + 0x3000; unsigned char* arr2 = arena + 0x4000;
    for (int iter = 0; iter < 200; ++iter) {
        memset(arena, 0, 0x2000);
        int w = iter & 1; int n1 = (iter >> 1) % 4; int n2 = (iter >> 3) % 4; int anim = ((iter >> 5) & 1) ? -1 : 0x66;
        *reinterpret_cast<int*>(self) = w ? 0x5555 : 0;
        *reinterpret_cast<unsigned*>(self + 96 + 8) = word_of(arr1); *reinterpret_cast<int*>(self + 96 + 16) = n1 ? 0 : -1;
        *reinterpret_cast<unsigned*>(self + 120 + 8) = word_of(arr2); *reinterpret_cast<int*>(self + 120 + 16) = n2 ? 0 : -1;
        for (int i = 0; i < 4; ++i) {
            *reinterpret_cast<int*>(arr1 + 16 * i) = 0x100 + i; *reinterpret_cast<int*>(arr1 + 16 * i + 8) = (i + 1 < n1) ? i + 1 : -1;
            *reinterpret_cast<int*>(arr2 + 104 * i) = 0x200 + i; *reinterpret_cast<int*>(arr2 + 104 * i + 96) = (i + 1 < n2) ? i + 1 : -1;
        }
        *reinterpret_cast<int*>(self + 384) = 0x77; *reinterpret_cast<int*>(self + 388) = anim;
        removeRet = (iter % 7 == 3) ? -1 : 0;
        int total = n1 + n2 + (anim != -1 ? 1 : 0) + 1;
        failAt = (iter % 3 == 0) ? 1 + (iter / 3) % total : 0;
        calls = 0; n_ = 0; expect_n = 0;
        int want = 0;
        if (w) { ex(1, (intptr_t)dict_, (intptr_t)self); }
        if (w && removeRet < 0) want = -1;
        else {
            int c = 0; bool stop = false;
            for (int i = 0; i < n1 && !stop; ++i) { ex(2, (intptr_t)scene_, 0x100 + i); if (++c == failAt) stop = true; }
            for (int i = 0; i < n2 && !stop; ++i) { ex(3, (intptr_t)scene_, 0x200 + i); if (++c == failAt) stop = true; }
            if (!stop && anim != -1) { ex(3, (intptr_t)scene_, anim); if (++c == failAt) stop = true; }
            if (!stop) { ex(4, (intptr_t)scene_, 0x77); if (++c == failAt) stop = true; }
            want = stop ? -1 : 0;
        }
        int got = Release__9VIHSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(self, scene_, dict_, 0, 0, 0);
        if (got != want || !same()) return iter + 1;
    }
    return 0;
}

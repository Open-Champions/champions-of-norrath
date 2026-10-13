// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "copy.cpp"
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
static int createId, initFail, shareFail, calls;
static P scene_, ns_;
extern "C" int CreateSprite__7VIScene12VISpriteTypei(P s, int t, int a) { rec(1, (intptr_t)s, t * 1000 + a); return createId; }
extern "C" unsigned Sprite__7VIScenei(P s, int i) { rec(2, (intptr_t)s, i); return word_of(ns_); }
static int init(int k, P d, const P s) { rec(10 + k, (intptr_t)d, (intptr_t)s); return k == initFail ? -2 : 0; }
extern "C" int Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode(unsigned char* d, const unsigned char* s) { return init(0, d, const_cast<unsigned char*>(s)); }
extern "C" int Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(unsigned char* d, const unsigned char* s) { return init(1, d, const_cast<unsigned char*>(s)); }
extern "C" int Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(unsigned char* d, const unsigned char* s) { return init(2, d, const_cast<unsigned char*>(s)); }
extern "C" int Init__t6VIPool1Z19VIHSpriteAttachmentRCt6VIPool1Z19VIHSpriteAttachment(unsigned char* d, const unsigned char* s) { return init(3, d, const_cast<unsigned char*>(s)); }
extern "C" int Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(unsigned char* d, const unsigned char* s) { return init(4, d, const_cast<unsigned char*>(s)); }
extern "C" int Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode(unsigned char* d, const unsigned char* s) { return init(5, d, const_cast<unsigned char*>(s)); }
static int tick(int id, int arg) { rec(id, (intptr_t)scene_, arg); return (++calls == shareFail) ? -4 : 3; }
extern "C" int ShareSprite__7VIScenei(P, int s) { return tick(3, s); }
extern "C" int ShareAnimation__7VIScenei(P, int a) { return tick(4, a); }
extern "C" int ShareRefMap__7VIScenei(P, int m) { return tick(5, m); }
static void fill(unsigned char* p, int seed, int n) { for (int i = 0; i < n; ++i) p[i] = (unsigned char)(i * 31 + seed * 17 + (i >> 8)); }
int main() {
    unsigned char* self = arena + 0x100; scene_ = arena + 0x2000; ns_ = arena + 0x6000;
    unsigned char* want = arena + 0xa000;
    unsigned char* arr1 = arena + 0x3000; unsigned char* arr2 = arena + 0x4000;
    for (int iter = 0; iter < 240; ++iter) {
        fill(self, iter, 0x200); fill(ns_, iter + 100, 0x200);
        memcpy(want, ns_, 0x200);
        int n1 = (iter >> 1) % 4; int n2 = (iter >> 3) % 4; int anim = ((iter >> 5) & 1) ? -1 : 0x66;
        *reinterpret_cast<unsigned*>(self + 96 + 8) = word_of(arr1); *reinterpret_cast<int*>(self + 96 + 16) = n1 ? 0 : -1;
        *reinterpret_cast<unsigned*>(self + 120 + 8) = word_of(arr2); *reinterpret_cast<int*>(self + 120 + 16) = n2 ? 0 : -1;
        for (int i = 0; i < 4; ++i) {
            *reinterpret_cast<int*>(arr1 + 16 * i) = 0x100 + i; *reinterpret_cast<int*>(arr1 + 16 * i + 8) = (i + 1 < n1) ? i + 1 : -1;
            *reinterpret_cast<int*>(arr2 + 104 * i) = 0x200 + i; *reinterpret_cast<int*>(arr2 + 104 * i + 96) = (i + 1 < n2) ? i + 1 : -1;
        }
        *reinterpret_cast<int*>(self + 384) = 0x77; *reinterpret_cast<int*>(self + 388) = anim;
        *reinterpret_cast<int*>(self + 52) = 5 + iter;
        createId = (iter % 9 == 0) ? -3 : 1 + iter % 5;
        initFail = (iter % 4 == 1) ? (iter / 4) % 6 : -1;
        int total = n1 + n2 + 2;
        shareFail = (iter % 3 == 0) ? 1 + (iter / 3) % total : 0;
        calls = 0; n_ = 0; expect_n = 0;
        int want_ret = createId < 0 ? -1 : createId;
        ex(1, (intptr_t)scene_, (5 + iter) * 1000 + 77 + iter);
        if (createId >= 0) {
            ex(2, (intptr_t)scene_, createId);
            static const int offs[6] = {64, 76, 88, 96, 120, 144};
            bool stop = false;
            for (int k = 0; k < 6 && !stop; ++k) { ex(10 + k, (intptr_t)(ns_ + offs[k]), (intptr_t)(self + offs[k])); if (k == initFail) stop = true; }
            if (stop) want_ret = -1;
            else {
                memcpy(want + 8, self + 8, 48);
                memcpy(want + 168, self + 168, 16); memcpy(want + 208, self + 208, 4); memcpy(want + 212, self + 212, 12); memcpy(want + 224, self + 224, 12);
                memcpy(want + 236, self + 236, 20); memcpy(want + 256, self + 256, 12); memcpy(want + 268, self + 268, 20);
                memcpy(want + 384, self + 384, 8); memcpy(want + 288, self + 288, 12);
                int c = 0;
                for (int i = 0; i < n1 && !stop; ++i) { ex(3, (intptr_t)scene_, 0x100 + i); if (++c == shareFail) stop = true; }
                for (int i = 0; i < n2 && !stop; ++i) { ex(4, (intptr_t)scene_, 0x200 + i); if (++c == shareFail) stop = true; }
                if (!stop) { ex(4, (intptr_t)scene_, anim); if (++c == shareFail) stop = true; }
                if (!stop) { ex(5, (intptr_t)scene_, 0x77); if (++c == shareFail) stop = true; }
                if (stop) want_ret = -1;
            }
        }
        int got = Copy__9VIHSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(self, scene_, 0, 0, 0, 77 + iter);
        if (got != want_ret || !same() || memcmp(ns_, want, 0x200) != 0) return iter + 1;
    }
    return 0;
}

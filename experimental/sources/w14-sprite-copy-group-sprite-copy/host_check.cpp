// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "copy.cpp"
#include <cstdint>
#include <cstring>
alignas(16) static unsigned char arena[0x10000];
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
typedef unsigned char* P;
struct Call { int id; intptr_t a, b, c; };
static Call log_[64]; static int n_;
static Call expect_[64]; static int expect_n;
static void rec(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0) { log_[n_++] = Call{id, a, b, c}; }
static void ex(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0) { expect_[expect_n++] = Call{id, a, b, c}; }
static bool same() {
    if (n_ != expect_n) return false;
    for (int i = 0; i < n_; ++i)
        if (log_[i].id != expect_[i].id || log_[i].a != expect_[i].a || log_[i].b != expect_[i].b || log_[i].c != expect_[i].c) return false;
    return true;
}
static unsigned char *self_, *ns_, *want_, *scene_, *rast_, *snd_, *coll_, *kids_, *tabs_;
static int createId, dupAt, dupResult;
static int childResult[8];
extern "C" int CreateSprite__7VIScene12VISpriteTypei(P s, int t, int a) { rec(1, (intptr_t)s, t, a); return createId; }
extern "C" unsigned Sprite__7VIScenei(P s, int i) {
    rec(2, (intptr_t)s, i);
    if (i >= 0x100 && i < 0x108) return word_of(kids_ + 0x100 * (i - 0x100));
    return word_of(ns_);
}
extern "C" int CopySprite__7VISceneii(P s, int i, int a) { rec(4, (intptr_t)s, i, a); return i == dupAt ? dupResult : 0x900 + i; }
extern "C" void ShareSprite__7VIScenei(P s, int i) { rec(5, (intptr_t)s, i); }
static int slot(int k, P a, P b) { rec(3, (intptr_t)a, (intptr_t)b); return childResult[k]; }
static int f0(P a, P b) { return slot(0, a, b); }
static int f1(P a, P b) { return slot(1, a, b); }
static int f2(P a, P b) { return slot(2, a, b); }
static int f3(P a, P b) { return slot(3, a, b); }
static int f4(P a, P b) { return slot(4, a, b); }
extern "C" void* norrath_host_function(unsigned w) {
    static void* fns[] = {(void*)f0, (void*)f1, (void*)f2, (void*)f3, (void*)f4};
    return (w >= 1 && w <= 5) ? fns[w - 1] : nullptr;
}
static void fill(unsigned char* p, int seed) { for (int i = 0; i < 0x400; ++i) p[i] = (unsigned char)(i * 31 + seed * 17 + (i >> 8)); }
static void copy_range(int from, int to) { memcpy(want_ + from, self_ + from, to - from); }
int main() {
    self_ = arena + 0x1000; ns_ = arena + 0x2000; want_ = arena + 0x3000; scene_ = arena + 0x4000; rast_ = arena + 0x4010; snd_ = arena + 0x4020; coll_ = arena + 0x4030;
    kids_ = arena + 0x5000; tabs_ = arena + 0x8000;
    const int counts[] = {0, 1, 2, 5, -3, 3};
    for (int iter = 0; iter < 96; ++iter) {
        fill(self_, iter); fill(ns_, iter + 100); memcpy(want_, ns_, 0x400);
        createId = (iter % 6 == 0) ? -3 : (iter % 7) + 1;
        int cnt = counts[iter % 6 == 0 ? 0 : (iter / 6 + iter) % 6];
        *reinterpret_cast<int*>(self_ + 52) = 9 + iter;
        *reinterpret_cast<int*>(self_ + 64) = cnt;
        for (int k = 0; k < 5; ++k) *reinterpret_cast<int*>(self_ + 68 + 32 * k) = 0x100 + k;
        for (int k = 0; k < 5; ++k) {
            unsigned char* kid = kids_ + 0x100 * k; unsigned char* tab = tabs_ + 0x100 * k;
            memset(kid, 0xcc, 0x100);
            *reinterpret_cast<unsigned*>(kid + 60) = word_of(tab);
            memset(tab, 0, 0x40);
            short adj = static_cast<short>(((iter + k) % 3) * 8 - 8);
            *reinterpret_cast<short*>(tab + 16) = adj;
            *reinterpret_cast<unsigned*>(tab + 20) = k + 1;
            // neighbouring slots call a different function so a wrong slot is seen
            *reinterpret_cast<unsigned*>(tab + 12) = ((k + 1) % 5) + 1;
            *reinterpret_cast<unsigned*>(tab + 28) = ((k + 2) % 5) + 1;
            childResult[k] = (((iter >> 1) + k) % 3 == 0) ? 1 : (((iter + k) % 3 == 1) ? 0 : 2);
        }
        dupAt = -77; dupResult = 0;
        int failAt = -1;
        if ((iter % 4) == 3 && cnt > 0) { failAt = (iter / 4) % cnt; dupAt = 0x100 + failAt; dupResult = -1; }
        n_ = 0; expect_n = 0;
        ex(1, (intptr_t)scene_, 9 + iter, 77 + iter);
        int wantRet = createId < 0 ? -1 : createId;
        if (createId >= 0) {
            ex(2, (intptr_t)scene_, createId);
            copy_range(8, 56); copy_range(64, 228);
            bool failed = false;
            for (int k = 0; k < cnt && !failed; ++k) {
                ex(2, (intptr_t)scene_, 0x100 + k);
                short adj = *reinterpret_cast<short*>(tabs_ + 0x100 * k + 16);
                ex(3, (intptr_t)(kids_ + 0x100 * k + adj), (intptr_t)scene_);
                if (childResult[k] == 1) {
                    ex(4, (intptr_t)scene_, 0x100 + k, 77 + iter);
                    int d = (0x100 + k == dupAt) ? dupResult : 0x900 + 0x100 + k;
                    *reinterpret_cast<int*>(want_ + 68 + 32 * k) = d;
                    if (d == -1) { failed = true; wantRet = -1; }
                }
            }
            if (!failed) for (int k = 0; k < cnt; ++k) ex(5, (intptr_t)scene_, *reinterpret_cast<int*>(want_ + 68 + 32 * k));
        }
        int got = Copy__13VIGroupSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(self_, scene_, rast_, snd_, coll_, 77 + iter);
        if (got != wantRet || !same() || memcmp(ns_, want_, 0x400) != 0) return iter + 1;
    }
    return 0;
}

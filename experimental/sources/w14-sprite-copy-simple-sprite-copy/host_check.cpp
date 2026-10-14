// Host check only; not a build input.
#include "copy.cpp"
#include <cstdint>
#include <cstring>
typedef unsigned char* P;
struct Call { int id; intptr_t a, b, c; };
static Call log_[32]; static int n_;
static Call expect_[32]; static int expect_n;
[[maybe_unused]] static int rets[8];
static void rec(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0) { log_[n_++] = Call{id, a, b, c}; }
static void ex(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0) { expect_[expect_n++] = Call{id, a, b, c}; }
static bool same() {
    if (n_ != expect_n) return false;
    for (int i = 0; i < n_; ++i)
        if (log_[i].id != expect_[i].id || log_[i].a != expect_[i].a || log_[i].b != expect_[i].b || log_[i].c != expect_[i].c) return false;
    return true;
}
alignas(8) static unsigned char self_[2048], ns_[2048], want_[2048], scene_[8], rast_[8], snd_[8], coll_[8];
static int createId;
extern "C" int CreateSprite__7VIScene12VISpriteTypei(P s, int t, int a) { rec(1, (intptr_t)s, t, a); return createId; }
extern "C" P Sprite__7VIScenei(P s, int i) { rec(2, (intptr_t)s, i); return ns_; }
static void fill(unsigned char* p, int seed) { for (int i = 0; i < 2048; ++i) p[i] = (unsigned char)(i * 31 + seed * 17 + (i >> 8)); }
static void copy_range(int from, int to) { memcpy(want_ + from, self_ + from, to - from); }
// Starts one scenario: fills both objects, sets the creation result and records the expected leading calls and base copy.
static void begin(int iter) {
    fill(self_, iter); fill(ns_, iter + 100); memcpy(want_, ns_, 2048);
    createId = (iter % 5 == 0) ? -3 : (iter % 7) + 1;
    if (iter % 11 == 0) createId = 0;
    *reinterpret_cast<int*>(self_ + 52) = 9 + iter;
    n_ = 0; expect_n = 0;
    ex(1, (intptr_t)scene_, 9 + iter, 77 + iter);
    if (createId >= 0) { ex(2, (intptr_t)scene_, createId); copy_range(8, 56); }
}
static int finish(int got, int wantRet) { return got == wantRet && same() && memcmp(ns_, want_, 2048) == 0; }
extern "C" int ShareMaterialPal__8VIRasteri(P r, int f) { rec(4, (intptr_t)r, f); return rets[0]; }
extern "C" int SharePrimBuffer__8VIRasteri(P r, int f) { rec(5, (intptr_t)r, f); return rets[1]; }
extern "C" int ShareCollBuffer__9VICollidei(P c, int f) { rec(6, (intptr_t)c, f); return rets[2]; }
int main() {
    for (int iter = 0; iter < 64; ++iter) {
        begin(iter);
        P rs = (iter & 1) ? rast_ : 0; P sd = (iter & 2) ? snd_ : 0; (void)rs; (void)sd;
        const int rv[] = {0, 4, -1, -8};
        rets[0] = rv[iter & 3]; rets[1] = rv[(iter >> 2) & 3]; rets[2] = rv[(iter >> 4) & 3];
        int wantRet = createId < 0 ? -1 : createId;
        if (createId >= 0) {
            copy_range(72, 76); copy_range(64, 72);
            bool stop = false;
            if (rs) {
                ex(4, (intptr_t)rast_, *(int*)(self_ + 72));
                if (rets[0] < 0) stop = true;
                else { ex(5, (intptr_t)rast_, *(int*)(self_ + 64)); if (rets[1] < 0) stop = true; }
            }
            if (stop) wantRet = -1;
            else { ex(6, (intptr_t)coll_, *(int*)(self_ + 68)); if (rets[2] < 0) wantRet = -1; }
        }
        if (!finish(Copy__14VISimpleSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(self_, scene_, rs, sd, coll_, 77 + iter), wantRet)) return iter + 1;
    }
    return 0;
}

// Host check only; not a build input.
#include "release.cpp"
#include <cstdint>
typedef unsigned char* P;
struct Call { int id; intptr_t a, b, c; };
static Call log_[32]; static int n_;
static int rets[8];
static void rec(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0) { log_[n_++] = Call{id, a, b, c}; }
static int expect_n; static Call expect_[32];
static void ex(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0) { expect_[expect_n++] = Call{id, a, b, c}; }
static bool same() {
    if (n_ != expect_n) return false;
    for (int i = 0; i < n_; ++i)
        if (log_[i].id != expect_[i].id || log_[i].a != expect_[i].a || log_[i].b != expect_[i].b || log_[i].c != expect_[i].c) return false;
    return true;
}
static void reset() { n_ = 0; expect_n = 0; }
extern "C" int Remove__12VIDictionaryR10VIResource(P d, P r) { rec(1, (intptr_t)d, (intptr_t)r); return rets[0]; }
[[maybe_unused]] alignas(8) static unsigned char self_[256], scene_[8], dict_[8], rast_[8], snd_[8], coll_[8];
static int shrink;
extern "C" void ReleaseSprite__7VIScenei(P s, int i) { rec(2, (intptr_t)s, i); if (shrink) *(int*)(self_ + 68) = shrink; }
int main() {
    const int vals[] = {0, 5, -3};
    const int counts[] = {0, 1, 3, -2, 5};
    for (int w = 0; w < 2; ++w) for (int r0 : vals) for (int cnt : counts) for (int sh = 0; sh < 2; ++sh) {
        for (int i = 0; i < 256; ++i) self_[i] = (unsigned char)(i * 7 + 1);
        *(int*)self_ = w ? 0x4321 : 0; *(int*)(self_ + 68) = cnt;
        for (int i = 0; i < 5; ++i) *(int*)(self_ + 76 + 8 * i) = 0x1000 + i * 17;
        shrink = sh ? 2 : 0;
        rets[0] = r0; reset();
        int want = 0;
        if (w) { ex(1, (intptr_t)dict_, (intptr_t)self_); if (r0 < 0) want = -1; }
        if (!(w && r0 < 0)) {
            int limit = cnt;
            for (int i = 0; i < limit; ++i) { ex(2, (intptr_t)scene_, 0x1000 + i * 17); if (sh) limit = 2; }
        }
        int got = Release__11VILODSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(self_, scene_, dict_, rast_, snd_, coll_);
        if (got != want) return 1;
        if (!same()) return 2;
    }
    return 0;
}

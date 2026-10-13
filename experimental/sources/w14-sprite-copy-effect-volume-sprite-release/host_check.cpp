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
extern "C" void Free__14VIEffectVolume(P v) { rec(1, (intptr_t)v); }
extern "C" int Remove__12VIDictionaryR10VIResource(P d, P r) { rec(2, (intptr_t)d, (intptr_t)r); return rets[0]; }
[[maybe_unused]] alignas(8) static unsigned char self_[256], scene_[8], dict_[8], rast_[8], snd_[8], coll_[8];
int main() {
    const int vals[] = {0, 5, -3};
    for (int w = 0; w < 2; ++w) for (int r0 : vals) {
        for (int i = 0; i < 256; ++i) self_[i] = (unsigned char)(i * 7 + 1);
        *(int*)self_ = w ? 0x4321 : 0;
        rets[0] = r0; reset();
        int want = 0;
        ex(1, (intptr_t)(self_ + 88));
        if (w) { ex(2, (intptr_t)dict_, (intptr_t)self_); if (r0 < 0) want = -1; }
        int got = Release__20VIEffectVolumeSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(self_, scene_, dict_, rast_, snd_, coll_);
        if (got != want) return 1;
        if (!same()) return 2;
    }
    return 0;
}

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
extern "C" int IsPlaying__13VISoundDevicei(P s, int v) { rec(2, (intptr_t)s, v); return rets[1]; }
extern "C" void Stop__13VISoundDevicei(P s, int v) { rec(3, (intptr_t)s, v); }
extern "C" void ReleaseSound__13VISoundDeviceiR12VIDictionary(P s, int h, P d) { rec(4, (intptr_t)s, h, (intptr_t)d); }
[[maybe_unused]] alignas(8) static unsigned char self_[256], scene_[8], dict_[8], rast_[8], snd_[8], coll_[8];
int main() {
    const int vals[] = {0, 5, -3};
    const int voices[] = {-1, 0, 7};
    for (int w = 0; w < 2; ++w) for (int r0 : vals) for (int vo : voices) for (int pl = 0; pl < 2; ++pl) for (int hasS = 0; hasS < 2; ++hasS) {
        for (int i = 0; i < 256; ++i) self_[i] = (unsigned char)(i * 7 + 1);
        *(int*)self_ = w ? 0x4321 : 0; *(int*)(self_ + 64) = 0x55; *(int*)(self_ + 68) = vo;
        rets[0] = r0; rets[1] = pl ? 3 : 0; reset();
        P sd = hasS ? snd_ : 0;
        int want = 0, v68 = vo;
        if (w) { ex(1, (intptr_t)dict_, (intptr_t)self_); if (r0 < 0) want = -1; }
        if (!(w && r0 < 0)) {
            if (vo != -1) { ex(2, (intptr_t)sd, vo); if (pl) { ex(3, (intptr_t)sd, vo); v68 = -1; } }
            if (hasS) ex(4, (intptr_t)snd_, 0x55, (intptr_t)dict_);
        }
        int got = Release__13VISoundSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(self_, scene_, dict_, rast_, sd, coll_);
        if (got != want) return 1;
        if (!same()) return 2;
        if (*(int*)(self_ + 68) != ((w && r0 < 0) ? vo : v68)) return 3;
    }
    return 0;
}

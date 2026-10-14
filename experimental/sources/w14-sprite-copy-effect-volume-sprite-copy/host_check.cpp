// Host check only; not a build input.
#include "copy.cpp"
#include <cstdint>
#include <cstring>
typedef unsigned char* P;
struct Call { int id; intptr_t a, b, c; };
static Call log_[40]; static int n_;
static Call expect_[40]; static int expect_n;
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
static int bits(float f) { int i; memcpy(&i, &f, 4); return i; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(P s, int t, int a) { rec(1, (intptr_t)s, t, a); return createId; }
extern "C" P Sprite__7VIScenei(P s, int i) { rec(2, (intptr_t)s, i); return ns_; }
extern "C" void Init__20VIEffectVolumeSpriteRC6VIBBox(P s, const unsigned char* b) { rec(3, (intptr_t)s, (intptr_t)b); }
extern "C" void Init__14VIEffectVolumeRC6VIBBox(P v, const unsigned char* b) { rec(4, (intptr_t)v, (intptr_t)b); }
extern "C" int Type__C14VIEffectVolume(const unsigned char* v) { rec(5, (intptr_t)v); return 1000 + (int)(v - self_); }
extern "C" float Density__C14VIEffectVolume(const unsigned char* v) { rec(6, (intptr_t)v); return 2.5f; }
extern "C" void SetType__14VIEffectVolume18VIEffectVolumeTypef(P v, int t, float d) { rec(7, (intptr_t)v, t, bits(d)); }
extern "C" unsigned Flags__C14VIEffectVolume(const unsigned char* v) { rec(8, (intptr_t)v); return 0xf0f0u + (unsigned)(v - self_); }
extern "C" void SetFlags__14VIEffectVolumeUi(P v, unsigned f) { rec(9, (intptr_t)v, f); }
extern "C" int Surface__C14VIEffectVolumei(const unsigned char* v, int k) { rec(10, (intptr_t)v, k); return 70 + k; }
extern "C" void AttachSurface__14VIEffectVolumeii(P v, int k, int s) { rec(11, (intptr_t)v, k, s); }
static void fill(unsigned char* p, int seed) { for (int i = 0; i < 2048; ++i) p[i] = (unsigned char)(i * 31 + seed * 17 + (i >> 8)); }
static void copy_range(int from, int to) { memcpy(want_ + from, self_ + from, to - from); }
int main() {
    for (int iter = 0; iter < 20; ++iter) {
        fill(self_, iter); fill(ns_, iter + 100); memcpy(want_, ns_, 2048);
        createId = (iter % 5 == 0) ? -3 : (iter % 7) + 1;
        if (iter % 11 == 0) createId = 0;
        *reinterpret_cast<int*>(self_ + 52) = 9 + iter;
        n_ = 0; expect_n = 0;
        ex(1, (intptr_t)scene_, 9 + iter, 77 + iter);
        int wantRet = createId < 0 ? -1 : createId;
        if (createId >= 0) {
            ex(2, (intptr_t)scene_, createId);
            ex(3, (intptr_t)ns_, (intptr_t)(self_ + 8));
            copy_range(48, 56);
            ex(4, (intptr_t)(ns_ + 88), (intptr_t)(self_ + 8));
            ex(5, (intptr_t)(self_ + 88)); ex(6, (intptr_t)(self_ + 88));
            ex(7, (intptr_t)(ns_ + 88), 1000 + 88, bits(2.5f));
            copy_range(88 + 52, 88 + 60);
            ex(8, (intptr_t)(self_ + 88)); ex(9, (intptr_t)(ns_ + 88), 0xf0f0u + 88);
            copy_range(88 + 48, 88 + 52); copy_range(88 + 40, 88 + 44);
            ex(10, (intptr_t)(self_ + 88), 0); ex(11, (intptr_t)(ns_ + 88), 0, 70);
            ex(10, (intptr_t)(self_ + 88), 1); ex(11, (intptr_t)(ns_ + 88), 1, 71);
        }
        int got = Copy__20VIEffectVolumeSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(self_, scene_, rast_, snd_, coll_, 77 + iter);
        if (got != wantRet || !same() || memcmp(ns_, want_, 2048) != 0) return iter + 1;
    }
    return 0;
}

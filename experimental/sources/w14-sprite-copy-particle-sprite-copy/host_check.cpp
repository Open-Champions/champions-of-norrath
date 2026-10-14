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
alignas(8) static unsigned char self_[2048], ns_[2048], want_[2048], scene_[4096], rast_[8], snd_[8], coll_[8];
static int createId;
extern "C" int CreateSprite__7VIScene12VISpriteTypei(P s, int t, int a) { rec(1, (intptr_t)s, t, a); return createId; }
extern "C" P Sprite__7VIScenei(P s, int i) { rec(2, (intptr_t)s, i); return ns_; }
extern "C" void Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition(P e, unsigned s, unsigned d) { rec(3, (intptr_t)e, s, d); }
extern "C" const unsigned char* Location__C17VIParticleEmitter(const unsigned char* e) { return e + 16; }
extern "C" void SetLocation__17VIParticleEmitterRC7VIVect3(P e, const unsigned char* l) { rec(4, (intptr_t)e, (intptr_t)l); }
extern "C" const unsigned char* Direction__C17VIParticleEmitter(const unsigned char* e) { return e + 40; }
extern "C" void SetDirection__17VIParticleEmitterRC10VIMatrix33(P e, const unsigned char* d) { rec(5, (intptr_t)e, (intptr_t)d); }
extern "C" const unsigned char* Space__C17VIParticleEmitter(const unsigned char* e) { return e + 96; }
extern "C" void SetSpace__17VIParticleEmitterRC10VIMatrix44(P e, const unsigned char* s) { rec(6, (intptr_t)e, (intptr_t)s); }
extern "C" void ShareParticleDefinition__16VIParticleSystemi(P s, int d) { rec(7, (intptr_t)s, d); }
static void fill(unsigned char* p, int seed) { for (int i = 0; i < 2048; ++i) p[i] = (unsigned char)(i * 31 + seed * 17 + (i >> 8)); }
static void copy_range(int from, int to) { memcpy(want_ + from, self_ + from, to - from); }
int main() {
    for (int iter = 0; iter < 40; ++iter) {
        fill(self_, iter); fill(ns_, iter + 100); memcpy(want_, ns_, 2048);
        createId = (iter % 5 == 0) ? -3 : (iter % 7) + 1;
        if (iter % 11 == 0) createId = 0;
        *reinterpret_cast<int*>(self_ + 52) = 9 + iter;
        const int defs[] = {-1, 0, 4, -2};
        int def = defs[iter & 3];
        *reinterpret_cast<int*>(self_ + 64) = def;
        n_ = 0; expect_n = 0;
        ex(1, (intptr_t)scene_, 9 + iter, 77 + iter);
        int wantRet = createId < 0 ? -1 : createId;
        if (createId >= 0) {
            ex(2, (intptr_t)scene_, createId);
            copy_range(1072, 1076); copy_range(8, 56);
            if (def != -1) {
                copy_range(64, 68);
                ex(3, (intptr_t)(ns_ + 80), *(unsigned*)(self_ + 160), *(unsigned*)(self_ + 164));
                ex(4, (intptr_t)(ns_ + 80), (intptr_t)(self_ + 96));
                ex(4, (intptr_t)(ns_ + 80), (intptr_t)(self_ + 96));
                ex(5, (intptr_t)(ns_ + 80), (intptr_t)(self_ + 120));
                ex(6, (intptr_t)(ns_ + 80), (intptr_t)(self_ + 176));
                ex(7, (intptr_t)(scene_ + 1352), def);
            }
        }
        int got = Copy__16VIParticleSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(self_, scene_, rast_, snd_, coll_, 77 + iter);
        if (got != wantRet || !same() || memcmp(ns_, want_, 2048) != 0) return iter + 1;
    }
    return 0;
}

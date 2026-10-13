// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "copy.cpp"
#include <cstdint>
#include <cstring>
alignas(16) static unsigned char arena[0x40000];
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
struct Call { int id; intptr_t a, b, c, d; };
static Call log_[512]; static int n_; static Call expect_[512]; static int expect_n;
static void rec(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0, intptr_t d = 0) { if (n_ < 512) log_[n_] = Call{id, a, b, c, d}; ++n_; }
static void ex(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0, intptr_t d = 0) { if (expect_n < 512) expect_[expect_n] = Call{id, a, b, c, d}; ++expect_n; }
static bool same() {
    if (n_ != expect_n || n_ > 512) return false;
    for (int i = 0; i < n_; ++i) if (log_[i].id != expect_[i].id || log_[i].a != expect_[i].a || log_[i].b != expect_[i].b || log_[i].c != expect_[i].c || log_[i].d != expect_[i].d) return false;
    return true;
}
struct Params {
    int cid, initfail, cmp, cpfail, cpval, smpfail, smpval, slfail, slval, ia, cpe, share, fa, fv, okv;
    bool raster; int types[3]; int n1, n2, lodn, lodtypes[3], cnt; int v4372[3];
};
static Params p;
static P self_, ns_, scene_, dict_, rast_, snd_, coll_, ar1_, ar2_, srcp_, dstp_, lodb_, want_;
static int cpCalls, smpCalls, slCalls, shCalls;
static int sprType(int id) { if (id >= 0x100 && id < 0x100 + p.n1) return p.types[(id - 0x100) % 3]; if (id >= 0x180 && id < 0x184) return p.lodtypes[(id - 0x180) % 3]; return 1; }
extern "C" int CreateSprite__7VIScene12VISpriteTypei(P s, int t, int a) { rec(1, (intptr_t)s, t, a); return p.cid; }
extern "C" unsigned Sprite__7VIScenei(P s, int i) { rec(2, (intptr_t)s, i); return word_of(i == p.cid ? ns_ : lodb_ + 0x200 * (i & 7)); }
static int initStub(int k, const unsigned char* d, const unsigned char* s) { rec(10 + k, (intptr_t)d, (intptr_t)s); return k == p.initfail ? -2 : 0; }
extern "C" int Init__t8VIVector1Z13VIHSpriteNodeRCt8VIVector1Z13VIHSpriteNode(unsigned char* d, const unsigned char* s) { return initStub(0, d, s); }
extern "C" int Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44(unsigned char* d, const unsigned char* s) { return initStub(1, d, s); }
extern "C" int Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(unsigned char* d, const unsigned char* s) { return initStub(2, d, s); }
extern "C" int Init__t6VIPool1Z19VIHSpriteAttachmentRCt6VIPool1Z19VIHSpriteAttachment(unsigned char* d, const unsigned char* s) { return initStub(3, d, s); }
extern "C" int Init__t6VIPool1Z13VIHSpritePlayRCt6VIPool1Z13VIHSpritePlay(unsigned char* d, const unsigned char* s) { return initStub(4, d, s); }
extern "C" int Init__t6VIPool1Z17VIHSpritePlayNodeRCt6VIPool1Z17VIHSpritePlayNode(unsigned char* d, const unsigned char* s) { return initStub(5, d, s); }
extern "C" int Init__t7VIArray1Z20VICSpritePartEmitteri(P a, int c) { rec(20, (intptr_t)a, c); return p.ia; }
extern "C" int CopyMaterialPal__8VIRasterii(P r, int pal, int a) { rec(21, (intptr_t)r, pal, a); return p.cmp; }
extern "C" int GetSpriteType__7VIScenei(P s, int i) { rec(22, (intptr_t)s, i); return sprType(i); }
extern "C" int CopySprite__7VISceneii(P s, int i, int a) { rec(23, (intptr_t)s, i, a); return ++cpCalls == p.cpfail ? p.cpval : 0x600 + (i & 0xff); }
extern "C" int SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary(P sp, int pal, P r, P d) { rec(24, (intptr_t)sp, pal, (intptr_t)r, (intptr_t)d); return ++smpCalls == p.smpfail ? p.smpval : 0; }
extern "C" int SetLevelSprite__11VILODSpriteiiR7VIScene(P l, int j, int s, P sc) { rec(25, (intptr_t)l, j, s, (intptr_t)sc); return ++slCalls == p.slfail ? p.slval : 0; }
extern "C" int CreateParticleEmitters__9VICSpriteR7VIScene(P s, P sc) { rec(26, (intptr_t)s, (intptr_t)sc); return p.cpe; }
extern "C" int Share__9VICSpriteR7VISceneP8VIRaster(P s, P sc, P r) { rec(27, (intptr_t)s, (intptr_t)sc, (intptr_t)r); return p.share; }
static int tick(int id, int arg) { rec(id, (intptr_t)scene_, arg); return ++shCalls == p.fa ? p.fv : p.okv; }
extern "C" int ShareSprite__7VIScenei(P, int s) { return tick(28, s); }
extern "C" int ShareAnimation__7VIScenei(P, int a) { return tick(29, a); }
static unsigned lcg = 7;
static unsigned rnd() { lcg = lcg * 1664525u + 1013904223u; return lcg >> 4; }
static void setw(unsigned char* b, int off, int v) { *reinterpret_cast<int*>(b + off) = v; }
static int getw(unsigned char* b, int off) { return *reinterpret_cast<int*>(b + off); }
static void cp(int from, int to) { memcpy(want_ + from, self_ + from, to - from); }
// Runs one scenario and returns nonzero on a mismatch.
static int run(int iter) {
    for (int i = 0; i < 0x1300; ++i) { self_[i] = (unsigned char)rnd(); ns_[i] = (unsigned char)rnd(); }
    memcpy(want_, ns_, 0x1300);
    setw(self_, 52, 9 + iter);
    for (int i = 0; i < 4; ++i) {
        setw(ar1_, 16 * i, 0x100 + i); setw(ar1_, 16 * i + 8, i + 1 < p.n1 ? i + 1 : -1);
        setw(ar2_, 104 * i, 0x200 + i); setw(ar2_, 104 * i + 96, i + 1 < p.n2 ? i + 1 : -1);
    }
    setw(ns_, 104, word_of(ar1_)); setw(ns_, 112, p.n1 ? 0 : -1); setw(want_, 104, word_of(ar1_)); setw(want_, 112, p.n1 ? 0 : -1);
    setw(self_, 128, word_of(ar2_)); setw(self_, 136, p.n2 ? 0 : -1);
    for (int q = 0; q < 8; ++q) { setw(lodb_, 0x200 * q + 68, p.lodn); for (int j = 0; j < 5; ++j) setw(lodb_, 0x200 * q + 76 + 8 * j, 0x180 + j); }
    setw(self_, 4616, p.cnt); setw(self_, 4620, word_of(srcp_)); setw(ns_, 4620, word_of(dstp_)); setw(want_, 4620, word_of(dstp_));
    for (int i = 0; i < 4 * 1056; ++i) { srcp_[i] = (unsigned char)rnd(); dstp_[i] = (unsigned char)rnd(); }
    unsigned char dstWant[4 * 1056]; memcpy(dstWant, dstp_, sizeof dstWant);
    for (int k = 0; k < 3; ++k) setw(self_, 4372 + 12 * k, p.v4372[k]);
    setw(scene_, 19352, word_of(dict_));
    cpCalls = smpCalls = slCalls = shCalls = 0; n_ = 0; expect_n = 0;
    const int arg = 0x31;
    P rs = p.raster ? rast_ : 0;
    int wantRet = -1; bool done = false;
    ex(1, (intptr_t)scene_, 9 + iter, arg);
    auto fail = [&]() { wantRet = -1; done = true; };
    if (p.cid < 0) fail();
    else {
        ex(2, (intptr_t)scene_, p.cid);
        static const int offs[6] = {64, 76, 88, 96, 120, 144};
        for (int k = 0; k < 6 && !done; ++k) { ex(10 + k, (intptr_t)(ns_ + offs[k]), (intptr_t)(self_ + offs[k])); if (k == p.initfail) fail(); }
    }
    if (!done) {
        cp(8, 56); cp(400, 404); cp(168, 184); cp(384, 392); cp(208, 212); cp(4424, 4436); setw(want_, 416, -1); cp(404, 416); cp(420, 424);
        cp(4436, 4456); cp(424, 3856); cp(3856, 3860); cp(3860, 3896); cp(3896, 4352); cp(4352, 4364); cp(4364, 4400); cp(4400, 4424); cp(4592, 4604); cp(4460, 4524);
        if (p.raster) { ex(21, (intptr_t)rast_, getw(self_, 416), arg); setw(want_, 416, p.cmp); if (p.cmp < 0) fail(); }
    }
    int cpc = 0, smc = 0, slc = 0, shc = 0;
    auto cpv = [&](int id) { return ++cpc == p.cpfail ? p.cpval : 0x600 + (id & 0xff); };
    auto smv = [&]() { return ++smc == p.smpfail ? p.smpval : 0; };
    auto slv = [&]() { return ++slc == p.slfail ? p.slval : 0; };
    int entries[4] = {0x100, 0x101, 0x102, 0x103};
    if (!done) {
        for (int i = 0; i < p.n1 && !done; ++i) {
            int id = entries[i];
            int t = sprType(id);
            ex(22, (intptr_t)scene_, id);
            bool copyBlock = false, lodBlock = false;
            if (t == 6) copyBlock = true;
            else { ex(22, (intptr_t)scene_, id); if (t == 3) copyBlock = true; else { ex(22, (intptr_t)scene_, id); if (t == 7) lodBlock = true; } }
            if (copyBlock) {
                ex(23, (intptr_t)scene_, id, 0); int dup = cpv(id);
                if (dup < 0) { fail(); break; }
                ex(22, (intptr_t)scene_, id);
                if (t == 3 && p.raster) {
                    ex(2, (intptr_t)scene_, dup); ex(24, (intptr_t)(lodb_ + 0x200 * (dup & 7)), p.cmp, (intptr_t)rast_, (intptr_t)dict_);
                    if (smv() < 0) { fail(); break; }
                }
                entries[i] = dup;
            } else if (lodBlock) {
                ex(23, (intptr_t)scene_, id, 0); int dup = cpv(id);
                if (dup < 0) { fail(); break; }
                ex(2, (intptr_t)scene_, dup);
                unsigned char* lod = lodb_ + 0x200 * (dup & 7);
                for (int j = 0; j < p.lodn && !done; ++j) {
                    int child = 0x180 + j;
                    ex(22, (intptr_t)scene_, child);
                    if (sprType(child) == 3) {
                        ex(23, (intptr_t)scene_, child, 0); int cc = cpv(child);
                        if (cc < 0) { fail(); break; }
                        if (p.raster) { ex(2, (intptr_t)scene_, cc); ex(24, (intptr_t)(lodb_ + 0x200 * (cc & 7)), p.cmp, (intptr_t)rast_, (intptr_t)dict_); if (smv() < 0) { fail(); break; } }
                        ex(25, (intptr_t)lod, j, cc, (intptr_t)scene_);
                        if (slv() < 0) { fail(); break; }
                    }
                }
                if (!done) entries[i] = dup;
            }
        }
    }
    if (!done) {
        ex(20, (intptr_t)(ns_ + 4616), p.cnt);
        if (p.ia < 0) fail();
    }
    if (!done) {
        for (int i = 0; i < p.cnt; ++i) {
            static const int words[8] = {0, 1020, 1012, 1008, 1024, 1016, 1028, 1032};
            for (int w : words) memcpy(dstWant + 1056 * i + w, srcp_ + 1056 * i + w, 4);
        }
        ex(26, (intptr_t)ns_, (intptr_t)scene_);
        if (p.cpe < 0) fail();
    }
    if (!done) {
        auto T = [&](int id, int arg2, bool used) { ex(id, (intptr_t)scene_, arg2); bool f = (++shc == p.fa); if (f && used && p.fv < 0) { fail(); } };
        for (int i = 0; i < p.n1 && !done; ++i) T(28, entries[i], true);
        for (int i = 0; i < p.n2 && !done; ++i) T(29, 0x200 + i, true);
        for (int k = 0; k < 3 && !done; ++k) if (p.v4372[k] != -1) T(28, p.v4372[k], false);
        if (!done) { ex(27, (intptr_t)ns_, (intptr_t)scene_, (intptr_t)rs); wantRet = p.share < 0 ? -1 : p.cid; }
    }
    int got = Copy__9VICSpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(self_, scene_, rs, snd_, coll_, arg);
    if (got != wantRet) return 1;
    if (!same()) return 2;
    for (int i = 0; i < p.n1; ++i) if (getw(ar1_, 16 * i) != entries[i]) return 3;
    if (memcmp(ns_, want_, 0x1300) != 0) return 4;
    if (memcmp(dstp_, dstWant, sizeof dstWant) != 0) return 5;
    return 0;
}
static Params base() {
    Params q = {5, -1, 7, 0, -1, 0, -3, 0, -2, 0, 0, 0, 0, -4, 3, true, {3, 6, 7}, 3, 2, 2, {3, 1, 3}, 2, {-1, 0x70, 0x71}};
    return q;
}
int main() {
    self_ = arena + 0x100; ns_ = arena + 0x2000; scene_ = arena + 0x4000; dict_ = arena + 0x9000; rast_ = arena + 0x9010; snd_ = arena + 0x9020; coll_ = arena + 0x9030;
    ar1_ = arena + 0xa000; ar2_ = arena + 0xb000; srcp_ = arena + 0xc000; dstp_ = arena + 0xe000; lodb_ = arena + 0x10000; want_ = arena + 0x20000;
    int iter = 0;
    auto go = [&](const Params& q) { p = q; int r = run(iter); if (r) return r * 1000 + iter; ++iter; return 0; };
    int r;
    if ((r = go(base()))) return r;
    { Params q = base(); q.cid = -3; if ((r = go(q))) return r; }
    static const int typeSets[6][3] = {{3, 6, 7}, {7, 7, 1}, {3, 3, 3}, {6, 6, 6}, {1, 1, 1}, {7, 3, 6}};
    for (int ra = 0; ra < 2; ++ra) for (int ts = 0; ts < 6; ++ts) for (int n1 = 0; n1 < 4; n1 += 1) for (int lodn = 0; lodn < 6; lodn += 2) {
        Params q = base(); q.raster = ra; memcpy(q.types, typeSets[ts], sizeof q.types); q.n1 = n1; q.lodn = lodn;
        if ((r = go(q))) return r;
    }
    for (int j = 0; j < 6; ++j) { Params q = base(); q.initfail = j; if ((r = go(q))) return r; }
    { Params q = base(); q.cmp = -1; if ((r = go(q))) return r; q.cmp = 0; q.raster = false; if ((r = go(q))) return r; }
    for (int pos = 1; pos < 7; ++pos) for (int v = -1; v <= 0; ++v) { Params q = base(); q.cpfail = pos; q.cpval = v * 9; memcpy(q.types, typeSets[pos % 6], sizeof q.types); if ((r = go(q))) return r; }
    for (int pos = 1; pos < 5; ++pos) for (int v = -3; v <= 0; v += 3) { Params q = base(); q.smpfail = pos; q.smpval = v; q.types[0] = 7; q.types[1] = 3; q.lodtypes[1] = 3; q.lodn = 3; if ((r = go(q))) return r; }
    for (int pos = 1; pos < 5; ++pos) for (int v = -2; v <= 0; v += 2) { Params q = base(); q.slfail = pos; q.slval = v; q.types[0] = 7; q.types[1] = 7; q.types[2] = 3; q.lodtypes[1] = 3; q.lodn = 3; if ((r = go(q))) return r; }
    { Params q = base(); q.ia = -1; if ((r = go(q))) return r; q.ia = -5; q.cnt = 0; if ((r = go(q))) return r; }
    for (int v = -6; v <= 0; v += 3) { Params q = base(); q.cpe = v; if ((r = go(q))) return r; }
    for (int cnt = -2; cnt < 4; ++cnt) { Params q = base(); q.cnt = cnt; if ((r = go(q))) return r; }
    for (int fa = 1; fa < 8; ++fa) for (int s = 0; s < 2; ++s) { Params q = base(); q.fa = fa; q.fv = s ? -1 : -4; q.okv = s ? 0 : 3; q.n1 = 3; q.n2 = 3; if ((r = go(q))) return r; }
    static const int shares[4] = {-1, -7, 0, 5};
    for (int s : shares) for (int ra = 0; ra < 2; ++ra) { Params q = base(); q.share = s; q.raster = ra; if ((r = go(q))) return r; }
    static const int v4372s[3][3] = {{-1, -1, -1}, {0x70, 0x71, 0x72}, {-1, 0x73, -1}};
    for (auto& v : v4372s) { Params q = base(); memcpy(q.v4372, v, sizeof q.v4372); if ((r = go(q))) return r; }
    return 0;
}


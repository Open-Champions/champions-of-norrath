#include <cstring>
#include <vector>

extern "C" {
unsigned char worldLastDrawSucceded;
unsigned char worldDrawEnable;
unsigned char modelClippingOn;
unsigned glistp;
unsigned gstart;
int GLISTSIZE;
alignas(8) unsigned char worldAnimation[64 * 48 + 8];
unsigned char texturePageTable[64];
alignas(4) unsigned char texturePageLoc[17 * 4];
void worldDrawWorld__FP12_worldHeader(void* world);
}

alignas(8) static unsigned char arena[32768];
unsigned char* norrath_world_arena = arena;

static unsigned& W(int off) { return *reinterpret_cast<unsigned*>(arena + off); }
static short& H(int off) { return *reinterpret_cast<short*>(arena + off); }
static float& F(int off) { return *reinterpret_cast<float*>(arena + off); }

struct FrustumCall { float b[6]; int mask; };
static std::vector<FrustumCall> frustum_calls;
static std::vector<int> clip_masks;
static std::vector<std::pair<unsigned char*, int>> draws;
static std::vector<std::pair<unsigned char*, int>> caches;
static int paused, paused_calls, anim_calls;
static std::vector<unsigned char*> anim_states;
static float reject_min_ge, reject_max_lt;
static int frustum_ret, clip_ret;

extern "C" int gameIsPaused__Fv() { ++paused_calls; return paused; }
extern "C" void animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(void* s, float t, float* p, void* uv, short* e) {
    ++anim_calls;
    anim_states.push_back(static_cast<unsigned char*>(s));
    if (t != 1.0f || p[0] != 0 || p[1] != 0 || p[2] != 0 || uv != nullptr || e != nullptr) anim_calls += 1000;
}
extern "C" int worldClipCubeToFrustum__FPA1_fi(float* b, int mask) {
    FrustumCall c;
    std::memcpy(c.b, b, sizeof c.b);
    c.mask = mask;
    frustum_calls.push_back(c);
    if (b[1] < reject_max_lt || b[0] >= reject_min_ge) return 0;
    return frustum_ret;
}
extern "C" int worldClipCubeToClipFrustum__FPA1_fi(float*, int mask) { clip_masks.push_back(mask); return clip_ret; }
extern "C" void drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(void* w, void* p, int flag) {
    if (w != arena) flag += 1000;
    draws.emplace_back(static_cast<unsigned char*>(p), flag);
}
extern "C" void texCacheData__FP8_texturei(void* t, int page) { caches.emplace_back(static_cast<unsigned char*>(t), page); }

static const int kRec = 8192;
static unsigned char* rec(int n) { return arena + kRec + n * 60; }
static void record(int n, float x0, float x1) {
    const int r = kRec + n * 60;
    std::memset(arena + r, 0, 60);
    F(r + 8) = x0; F(r + 20) = x1; F(r + 12) = 1; F(r + 24) = 2; F(r + 16) = 3; F(r + 28) = 4;
}
static void leaf(int at, int n, const int* idx) {
    H(at) = static_cast<short>(n);
    for (int i = 0; i < n; ++i) H(at + 2 + i * 2) = static_cast<short>(idx[i]);
}
static void interior(int at, int axis, int k1, int k2, int k3, int k4, int right_at) {
    H(at) = static_cast<short>(0xc000 | axis);
    H(at + 2) = static_cast<short>(k1); H(at + 4) = 0;
    H(at + 6) = static_cast<short>(k2); H(at + 8) = 0;
    H(at + 10) = static_cast<short>(k3); H(at + 12) = 0;
    H(at + 14) = static_cast<short>(k4); H(at + 16) = 0;
    H(at + 18) = static_cast<short>((right_at - (at + 18)) / 2);
}
static void reset() {
    frustum_calls.clear(); clip_masks.clear(); draws.clear(); caches.clear(); anim_states.clear();
    paused = 1; paused_calls = 0; anim_calls = 0;
    reject_min_ge = 1e30f; reject_max_lt = -1e30f;
    frustum_ret = 0x8003; clip_ret = 0x803f;
    worldDrawEnable = 1; worldLastDrawSucceded = 0; modelClippingOn = 1;
    gstart = 1000; glistp = 1000; GLISTSIZE = 100000;
}

int main() {
    std::memset(arena, 0, sizeof arena);
    W(4) = 2048; W(36) = kRec;
    for (int i = 0; i < 8; ++i) record(i, 0.0f, 5000.0f);

    // Tree A: root interior at 2048 (axis 0), left leaf {0} at +20, right leaf {1}.
    const int root = 2048;
    interior(root, 0, 50, 40, -5, 10, root + 40);
    const int l0[1] = {0};
    const int r1[1] = {1};
    leaf(root + 20, 1, l0);
    leaf(root + 40, 1, r1);

    // Null world: flag set, nothing else touched.
    reset();
    worldDrawWorld__FP12_worldHeader(nullptr);
    if (worldLastDrawSucceded != 1 || paused_calls != 0 || modelClippingOn != 1 || !frustum_calls.empty()) return 1;

    // Both children visible; the right child is processed first.
    reset();
    worldDrawWorld__FP12_worldHeader(arena);
    if (worldLastDrawSucceded != 1 || modelClippingOn != 0 || paused_calls != 1) return 2;
    if (frustum_calls.size() != 4) return 3;
    const FrustumCall& lc = frustum_calls[0];
    const FrustumCall& rc = frustum_calls[1];
    if (lc.mask != 0x8000 || rc.mask != 0x8000) return 4;
    if (lc.b[0] != -1000000.0f || lc.b[1] != 200.0f || lc.b[2] != -1000000.0f || lc.b[3] != 1000000.0f) return 5;
    if (lc.b[4] != -20.0f || lc.b[5] != 40.0f) return 6;
    if (rc.b[0] != 160.0f || rc.b[1] != 1000000.0f || rc.b[4] != -20.0f) return 7;
    if (draws.size() != 2 || draws[0].first != rec(1) || draws[1].first != rec(0)) return 8;
    if (draws[0].second != 0 || draws[1].second != 0) return 9;
    // Leaf clip calls use the entry mask returned by the node clip, and the record box order.
    if (frustum_calls[2].mask != 0x8003 || frustum_calls[2].b[0] != 0.0f || frustum_calls[2].b[1] != 5000.0f) return 10;
    if (frustum_calls[2].b[2] != 1.0f || frustum_calls[2].b[3] != 2.0f || frustum_calls[2].b[4] != 3.0f || frustum_calls[2].b[5] != 4.0f) return 11;
    if (clip_masks.size() != 2 || clip_masks[0] != 0x8003) return 12;

    // Not fully inside the second frustum: the flag becomes 1.
    reset(); clip_ret = 0x8003;
    worldDrawWorld__FP12_worldHeader(arena);
    if (draws.size() != 2 || draws[0].second != 1 || draws[1].second != 1) return 13;

    // Right box rejected: only the left side is drawn. Its box keeps the original min.
    reset(); reject_min_ge = 100.0f;
    // the leaf records must not be rejected by this test
    for (int i = 0; i < 8; ++i) record(i, -50.0f, 5000.0f);
    worldDrawWorld__FP12_worldHeader(arena);
    if (draws.size() != 1 || draws[0].first != rec(0)) return 14;
    // Left rejected: only right, whose box keeps min = key2 scaled.
    reset(); reject_max_lt = 300.0f;
    worldDrawWorld__FP12_worldHeader(arena);
    if (draws.size() != 1 || draws[0].first != rec(1)) return 15;
    if (frustum_calls[2].mask != 0x8003) return 16;
    // Neither visible.
    reset(); reject_max_lt = 1e9f;
    worldDrawWorld__FP12_worldHeader(arena);
    if (!draws.empty() || modelClippingOn != 0 || frustum_calls.size() != 2) return 17;

    // Tree B: root -> left interior N1 (two leaves), right leaf. With a fully inside
    // result the nested node and the leaves use no further clip calls.
    reset();
    for (int i = 0; i < 8; ++i) record(i, 0.0f, 5000.0f);
    const int n1 = root + 20;
    interior(root, 0, 50, 40, -5, 10, root + 120);
    interior(n1, 1, 7, 8, 9, 10, n1 + 40);
    const int l2[1] = {2}, r3[1] = {3};
    leaf(n1 + 20, 1, l2);
    leaf(n1 + 40, 1, r3);
    const int r1b[1] = {1};
    leaf(root + 120, 1, r1b);
    frustum_ret = 0x803f;
    worldDrawWorld__FP12_worldHeader(arena);
    if (frustum_calls.size() != 2) return 18;
    if (!clip_masks.empty()) return 19;
    if (draws.size() != 3 || draws[0].first != rec(1) || draws[1].first != rec(3) || draws[2].first != rec(2)) return 20;
    for (auto& d : draws) if (d.second != 0) return 21;

    // Texture pass (worldDrawEnable clear).
    reset(); worldDrawEnable = 0;
    std::memset(texturePageTable, 0xff, sizeof texturePageTable);
    texturePageTable[8] = 2; texturePageTable[9] = 2;
    *reinterpret_cast<unsigned*>(texturePageLoc + 2 * 4) = 3000;
    // Root is a leaf of five records.
    const int five[5] = {0, 1, 2, 3, 4};
    leaf(root, 5, five);
    W(kRec + 0 * 60 + 36) = 0x80;                       // skipped by flag
    H(kRec + 1 * 60 + 52) = 0;                          // page zero: skipped
    H(kRec + 2 * 60 + 52) = 7;                          // not loaded
    H(kRec + 3 * 60 + 52) = 8; W(kRec + 3 * 60 + 32) = 16;
    H(kRec + 4 * 60 + 52) = 9; W(kRec + 4 * 60 + 32) = 48;
    H(3000 + 16 + 8) = 0x100; W(3000 + 16 + 20) = 0;    // eligible
    H(3000 + 48 + 8) = 0x000;                           // not flagged
    frustum_ret = 0x803f;
    worldDrawWorld__FP12_worldHeader(arena);
    if (!draws.empty()) return 22;
    if (caches.size() != 1 || caches[0].first != arena + 3016 || caches[0].second != 8) return 23;
    if (worldLastDrawSucceded != 0) return 24;
    // Same pass with only a record that has a nonzero word at +20: no request, flag stays 1.
    reset(); worldDrawEnable = 0;
    const int one[1] = {3};
    leaf(root, 1, one);
    W(3000 + 16 + 20) = 5;
    worldDrawWorld__FP12_worldHeader(arena);
    if (!caches.empty() || worldLastDrawSucceded != 1) return 25;
    // Texture base plus word at +32 equal to zero is skipped.
    W(3000 + 16 + 20) = 0;
    *reinterpret_cast<unsigned*>(texturePageLoc + 2 * 4) = 0;
    W(kRec + 3 * 60 + 32) = 0;
    reset(); worldDrawEnable = 0;
    worldDrawWorld__FP12_worldHeader(arena);
    if (!caches.empty() || worldLastDrawSucceded != 1) return 26;

    // Display list guard.
    for (int i = 0; i < 8; ++i) record(i, 0.0f, 5000.0f);
    reset(); frustum_ret = 0x803f;
    leaf(root, 2, five);
    gstart = 0; glistp = 8 * 10;       // 10 entries used
    GLISTSIZE = 3009;                    // 2999 remaining
    worldDrawWorld__FP12_worldHeader(arena);
    if (!draws.empty() || modelClippingOn != 1) return 27;
    reset(); frustum_ret = 0x803f;
    gstart = 0; glistp = 8 * 10; GLISTSIZE = 3010;  // exactly 3000 remaining
    worldDrawWorld__FP12_worldHeader(arena);
    if (draws.size() != 2 || modelClippingOn != 0) return 28;

    // Animation records.
    reset(); paused = 0; frustum_ret = 0x803f;
    std::memset(worldAnimation, 0, sizeof worldAnimation);
    worldAnimation[0 * 48 + 4] = 1; worldAnimation[1 * 48 + 4] = 2; worldAnimation[2 * 48 + 4] = 0x80;
    worldAnimation[4 * 48 + 4] = 1;  // after a gap: not visited
    leaf(root, 0, five);
    worldDrawWorld__FP12_worldHeader(arena);
    if (anim_calls != 3 || anim_states.size() != 3) return 29;
    for (int i = 0; i < 3; ++i) if (anim_states[i] != worldAnimation + i * 48) return 30;
    // Paused: none.
    reset(); paused = 1; anim_calls = 0;
    worldDrawWorld__FP12_worldHeader(arena);
    if (anim_calls != 0) return 31;
    // 64 records: scan stops after 64.
    reset(); paused = 0;
    for (int i = 0; i < 64; ++i) worldAnimation[i * 48 + 4] = 1;
    worldDrawWorld__FP12_worldHeader(arena);
    if (anim_calls != 64) return 32;
    return 0;
}

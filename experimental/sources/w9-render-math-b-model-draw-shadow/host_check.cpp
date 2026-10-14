// Host contract for modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec.
// Not an EE plan input. Build with -ffp-contract=off. Every callee is a host
// stand-in that records its arguments; the display list is a host buffer that is
// pre-filled with 0xa5 so unwritten or unpadded bytes show up.
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "../w9-render-math-b-common/math_types.hpp"

struct AnimationState { int tag; };
struct WorldHeader { int tag; };
struct Texture { int tag; };
struct DrawRecord { int tag; };
struct ShadowPacket { unsigned char* cursor; unsigned char* last; };

extern "C" {
alignas(16) unsigned char listBuffer[0x4000];
unsigned char* glistp;
WorldHeader* world;
float modelShadowScale, shadowBrightness;
int engineShadowTexAddress, engineEnvTexAddress, engineDrawBufferStartAddress, engineYOffset, engineScissory1, engineScissory2;
float worldCameraPos[3], worldViewMatrix[16], worldCameraMatrix[12];
char engineCurrentLevelName[80];
}

static std::vector<std::string> trace;
static std::vector<float> findResults;
static size_t findAt;
static std::vector<Point3> findProbes;
static signed char lightBytes[16];
static float scaleSeen;
static Matrix44 viewSeen;
static float transArgs[3];
static int ctorCalls, getCalls, drawCalls, ctorTotal;
static unsigned char* drawList; static int drawValue; static Texture* drawTexture; static DrawRecord* drawRecord; static int drawExtra;
static AnimationState* animSeen;
static unsigned char* animCursor;
static std::string ctorName1, ctorName2;
static int nStrcmp;

extern "C" {
float worldFindHeight__FP12_worldHeaderG6Point3f(WorldHeader* w, Point3 p, float f) {
    trace.push_back("find"); findProbes.push_back(p);
    if (w != world || f != 5.0f) trace.push_back("BAD find args");
    return findResults[findAt++ % findResults.size()];
}
void lightCharacterShadow__FG6Point3P11LightResult(Point3, void* out) { trace.push_back("light"); std::memcpy(out, lightBytes, 12); }
void animLoadBoneMatricies__FP6PacketP14AnimationState(ShadowPacket* p, AnimationState* a) { trace.push_back("anim"); animSeen = a; animCursor = p->cursor; p->cursor += 32; }
void matScale__FP8Matrix44f(Matrix44*, float f) { trace.push_back("scale"); scaleSeen = f; }
void matTrans__FP8Matrix44fff(Matrix44* m, float x, float y, float z) { trace.push_back("trans"); transArgs[0] = x; transArgs[1] = y; transArgs[2] = z; viewSeen = *m; }
// Stand-in transform: x' = x + 1, y' = 2 * y, z' = z + 5; w = 3 + x.
float matTransform__FP6Point3P8Matrix44T0(Point3* out, const Matrix44*, const Point3* in) {
    trace.push_back("xform"); *out = Point3{in->x + 1.0f, in->y * 2.0f, in->z + 5.0f}; return 3.0f + in->x;
}
int strcmp(const char* a, const char* b) {
    ++nStrcmp;
    while (*a && *a == *b) { ++a; ++b; }
    return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
}
void* __11ResourceMapPcT1(void* self, const char* a, const char* b) { ++ctorCalls; ++ctorTotal; ctorName1 = a; ctorName2 = b; trace.push_back("ctor"); return self; }
Texture* get__11ResourceMap(void*) { ++getCalls; static Texture t; return &t; }
void drawAddToList__FPviP8_textureP11_drawRecordi(void* l, int v, Texture* t, DrawRecord* r, int e) {
    ++drawCalls; drawList = static_cast<unsigned char*>(l); drawValue = v; drawTexture = t; drawRecord = r; drawExtra = e; trace.push_back("draw");
}
void modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(const unsigned char*, int, const float*, AnimationState*, signed char);
}

static unsigned long long q64(const unsigned char* p) { unsigned long long v; std::memcpy(&v, p, 8); return v; }
static unsigned q32(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static unsigned fbits(float f) { unsigned v; std::memcpy(&v, &f, 4); return v; }
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

static void reset() {
    std::memset(listBuffer, 0xa5, sizeof listBuffer);
    glistp = listBuffer; trace.clear(); findProbes.clear(); findAt = 0; ctorCalls = getCalls = drawCalls = 0; nStrcmp = 0;
    animSeen = nullptr; world = nullptr; std::strcpy(engineCurrentLevelName, "elsewhere");
    modelShadowScale = 2.0f; shadowBrightness = 100.0f;
    engineShadowTexAddress = 0x123400; engineEnvTexAddress = 0x234500; engineDrawBufferStartAddress = 0x345600;
    engineYOffset = 0x700; engineScissory1 = 3; engineScissory2 = 0x1ff;
    for (float& f : worldViewMatrix) f = 0; for (float& f : worldCameraMatrix) f = 0; for (float& f : worldCameraPos) f = 0;
    worldViewMatrix[2] = worldViewMatrix[6] = 0; worldViewMatrix[10] = 1;
    worldCameraMatrix[2] = 0.5f; worldCameraMatrix[5] = -0.25f; worldCameraMatrix[8] = 1.0f;
    std::memset(lightBytes, 0, sizeof lightBytes);
    findResults = {1.0f};
}

int main() {
    alignas(16) unsigned char model[256] = {};
    model[36] = 2;                                   // table at model + 32
    unsigned tableCount = 0x00000123u;
    std::memcpy(model + 32, &tableCount, 4);
    float m[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10.0f, 20.0f, 30.0f};
    AnimationState anim{1};

    // Zero byte at model+36: nothing at all happens.
    {
        reset();
        unsigned char none[64] = {};
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(none, 5, m, nullptr, -1);
        expect(trace.empty() && glistp == listBuffer && drawCalls == 0 && listBuffer[0] == 0xa5, "empty model draws nothing");
    }

    // Cull test: the object is returned from when -3000 < dot.
    {
        reset();
        worldCameraPos[2] = 0;                       // dot = 30 * 1 = 30
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 5, m, nullptr, 1);
        expect(trace.empty() && drawCalls == 0, "mode 1 culls when -3000 < dot");
        reset();
        worldCameraPos[2] = 4000.0f;                 // dot = 30 - 4000 = -3970
        lightBytes[2] = 50;
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 5, m, nullptr, 1);
        expect(drawCalls == 1, "mode 1 draws when dot <= -3000");
        reset();
        lightBytes[2] = 50;
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 5, m, nullptr, -1);
        expect(drawCalls == 1, "mode -1 never culls");
    }

    // Full walk with one light, no animation state, mode -1.
    {
        reset();
        lightBytes[0] = 16; lightBytes[1] = -32; lightBytes[2] = 50;
        lightBytes[5] = 10; lightBytes[8] = 20; lightBytes[11] = 30;
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 77, m, nullptr, -1);
        const unsigned char* b = listBuffer;
        // packet 1: DMA CNT qwc 13, FLUSH, DIRECT 13, GIF tag with 12 entries
        expect(q64(b) == (0x10000000ull | 13) && q32(b + 8) == 0x11000000u && q32(b + 12) == (0x50000000u | 13), "packet 1 tags");
        expect(q64(b + 16) == (0x1000000000008000ull | 12) && q64(b + 24) == 14, "packet 1 GIF tag");
        const unsigned long long regs1[12] = {63, 6, 77, 72, 25, 65, 68, 7, 21, 67, 71, 66};
        const unsigned long long vals1[12] = {0, 0, 0x2020000, 0x31001, (31744ull << 32) | 0x7c00, 0x7f0000007f0000ull, 0x1212303012123030ull,
                                               0, 1, (128ull << 32) | 0x68, 0x51001, (128ull << 32) | 0xa1};
        bool ok = true;
        for (int i = 0; i < 12; ++i) {
            ok = ok && q64(b + 32 + 16 * i + 8) == regs1[i];
            if (i != 7) ok = ok && q64(b + 32 + 16 * i) == vals1[i];
        }
        expect(ok, "packet 1 entries");
        const unsigned long long tex0 = 0x1234ull | 0x14004000ull | (0x8000ull << 46) | (0xa000ull << 17);
        expect(q64(b + 32 + 16 * 7) == tex0, "packet 1 TEX0 from engineShadowTexAddress");
        const unsigned char* cnt = b + 16 * 14;      // MSCAL tag
        expect(q64(cnt) == 0x10000000ull && q64(cnt + 8) == 0x1400001eull, "MSCAL 0x1e tag when there is no animation state");
        const unsigned char* chunk = cnt + 16;
        expect(q64(chunk) == (0x10000000ull | 7) && q32(chunk + 8) == 0x13000000u && q32(chunk + 12) == 0x04000300u && q32(chunk + 16) == 0x6c054300u, "matrix chunk header");
        expect(q32(chunk + 84) == 0x8000u && q32(chunk + 88) == 0x41ab4000u && q32(chunk + 92) == 21331u && q32(chunk + 96) == 0u && q32(chunk + 100) == 0x60014305u, "matrix chunk constants");
        expect(q32(chunk + 104) == fbits(0.5f * __builtin_bit_cast(float, 0x3c03126eu)) && q32(chunk + 108) == 0x1400002eu && q32(chunk + 112) == 0x13000000u, "matrix chunk tail (scale constant, MSCAL 46)");
        expect(q32(chunk + 116) == 0 && q32(chunk + 120) == 0 && q32(chunk + 124) == 0, "chunk padded with zeros to a quadword");
        expect(fbits(scaleSeen) == fbits(0.5f * 1.5f), "matScale gets 1.5 / modelShadowScale");
        const float lx = static_cast<float>(-16) * 0.0078125f, ly = static_cast<float>(32) * 0.0078125f;
        const float C2 = __builtin_bit_cast(float, 0x3eaaaaa9u);
        const float want[16] = {1, 2, 3, 0, 4, 5, 6, 0, 7 + lx * C2, 8 + ly * C2, 9, 0, __builtin_bit_cast(float, 0x49008000u), __builtin_bit_cast(float, 0x49008000u), 0, 1};
        bool mat = true;
        for (int i = 0; i < 16; ++i) mat = mat && q32(chunk + 20 + 4 * i) == fbits(want[i]);
        expect(mat, "uploaded matrix rows");
        const unsigned char* sprite = chunk + 128;
        expect(q64(sprite) == (0x10000000ull | 8) && q32(sprite + 12) == (0x50000000u | 8) && q64(sprite + 16) == (0x1000000000008000ull | 7), "sprite packet tags");
        const unsigned long long sprVals[7] = {0x80000000ull, 518, 0x7c007c00ull, 0x84008000ull, 0x7c008000ull, 0x84008400ull, 0x80808080ull};
        const unsigned long long sprRegs[7] = {1, 0, 5, 5, 5, 5, 1};
        bool spr = true;
        for (int i = 0; i < 7; ++i) spr = spr && q64(sprite + 32 + 16 * i) == sprVals[i] && q64(sprite + 40 + 16 * i) == sprRegs[i];
        expect(spr, "sprite packet entries");
        const unsigned char* ref = sprite + 144;
        const unsigned long long refWant = 0x123ull | 0x30000000ull | (static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(model + 48)) << 32);
        expect(q64(ref) == refWant && q64(ref + 8) == 0, "REF tag: count word and table + 16");
        const unsigned char* quad = ref + 16;
        expect(q64(quad) == (0x10000000ull | 16) && q32(quad + 12) == (0x50000000u | 16) && q64(quad + 16) == (0x1000000000008000ull | 15), "textured packet tags");
        expect(q64(quad + 32) == 0 && q64(quad + 40) == 63, "TEXFLUSH");
        const unsigned shade = 50u * 100u;                       // (int)(50 * 100.0f)
        const unsigned colour = shade | (shade << 8) | (shade << 16) | 0x80000000u;
        expect(q64(quad + 48) == colour && q64(quad + 56) == 1, "colour entry");
        // The stand-in transform sends every corner to a point that passes the range test, except
        // that the corner z of the stand-in is positive, so the primitive stays type 84.
        expect(q64(quad + 64) == 84 || q64(quad + 64) == 80, "primitive entry is 84 or 80");
        const float C5 = __builtin_bit_cast(float, 0x3eaaaaaau);
        const float groundZ = 30.0f;                              // no world: ground is the translation z
        expect(fbits(transArgs[0]) == fbits(10.0f + lx * (30.0f - groundZ) * C5) && fbits(transArgs[2]) == fbits(0.0f), "view translated by the light shift");
        const float extent = 2.0f * 42.0f;
        const float back0 = 0.5f * 12.0f, back1 = -0.25f * 12.0f, back2 = 1.0f * 12.0f;
        const float cx[4] = {-extent, -extent, extent, extent}, cy[4] = {-extent, extent, -extent, extent};
        bool corners = true, st = true, inside = true;
        const float C7 = __builtin_bit_cast(float, 0x358637bdu), C8 = __builtin_bit_cast(float, 0x3f7fffefu);
        for (int k = 0; k < 4; ++k) {
            const float px = (cx[k] - back0) + 1.0f, py = (cy[k] - back1) * 2.0f, pz = (groundZ - back2) + 5.0f;
            const float w = 3.0f + (cx[k] - back0);
            const float q = 1.0f / (w + 12.0f);
            const unsigned char* e = quad + 80 + 48 * k;
            float s0 = q * C7, s1 = q * C7;
            if (k == 1) s1 = q * C8;
            if (k == 2) { s0 = q * __builtin_bit_cast(float, 0x3f7fffefu); s1 = C7; }
            if (k == 3) { s0 = s1 = q * __builtin_bit_cast(float, 0x3f7fffefu); }
            st = st && q64(e) == ((static_cast<unsigned long long>(fbits(s1)) << 32) | fbits(s0)) && q64(e + 8) == 2;
            corners = corners && q64(e + 16) == (colour | (static_cast<unsigned long long>(fbits(q)) << 32)) && q64(e + 24) == 1;
            const int ix = static_cast<int>(px * q * 16.0f) + 0x8000, iy = static_cast<int>(py * q * 16.0f) + 0x8000, iz = static_cast<int>((pz / w) * 16.0f);
            if (iz < 0 || ix < 0 || ix > 0xffff || iy < 0 || iy > 0xffff) inside = false;
            const unsigned long long xyz = static_cast<unsigned long long>(static_cast<long long>(ix)) | (static_cast<unsigned long long>(static_cast<long long>(iy)) << 16) |
                                           (static_cast<unsigned long long>(static_cast<long long>(iz)) << 32);
            corners = corners && q64(e + 32) == xyz && q64(e + 40) == 5;
        }
        expect(st, "ST entries per corner (each corner has its own constants; corner 2 keeps its second coordinate unscaled)");
        expect(corners, "RGBAQ and XYZ2 entries per corner");
        expect(q64(quad + 64) == (inside ? 84ull : 80ull), "primitive reflects the range test");
        const unsigned char* last = quad + 16 * 17;
        expect(q64(last) == (0x60000008ull) && q32(last + 12) == (0x50000000u | 8) && q64(last + 16) == (0x1000000000008000ull | 7), "restore packet tags (CNT turned into id 6)");
        const unsigned long long drawBuf = (static_cast<unsigned long long>(0x345600) >> 13) | 0x2140000ull;
        expect(q64(last + 32) == drawBuf && q64(last + 40) == 77 && q64(last + 48) == 0x50013ull && q64(last + 56) == 72, "restore frame and test");
        expect(q64(last + 64) == (0x234500ull >> 8 | 0x1c008000ull | (0x8000ull << 46) | (0xe000ull << 17)) && q64(last + 72) == 7, "restore TEX0");
        expect(q64(last + 80) == 0x3131757531317575ull && q64(last + 96) == 97 && q64(last + 104) == 21, "restore dither and TEX1");
        expect(q64(last + 112) == ((0x700ull << 32) | 22528) && q64(last + 120) == 25, "restore offset");
        expect(q64(last + 128) == (0x4ff0000ull | (3ull << 32) | (0x1ffull << 48)) && q64(last + 136) == 65, "restore scissor");
        expect(glistp == last + 144, "glistp is the cursor");
        expect(drawCalls == 1 && drawList == listBuffer && drawValue == 77 && drawRecord == nullptr && drawExtra == 16 && drawTexture != nullptr, "drawAddToList arguments");
        expect(getCalls == 1 && ctorTotal == 1 && ctorName1 == "engine.lmp" && ctorName2 == "bigblank.tex", "resource built once, on the first drawing call, from the two names");
        expect(nStrcmp == 1, "level name compared once");
    }

    // Second call: the resource map is not rebuilt.
    {
        reset(); lightBytes[2] = 3;
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, nullptr, -1);
        expect(ctorTotal == 1 && getCalls == 1, "static resource built only on the first call, fetched every call");
    }

    // Animation state: the bone loader is called and no MSCAL tag is appended; matrix chunk uses 44.
    {
        reset(); lightBytes[2] = 3;
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, &anim, -1);
        expect(animSeen == &anim && animCursor == listBuffer + 16 * 14, "animLoadBoneMatricies receives the cursor after packet 1");
        const unsigned char* chunk = listBuffer + 16 * 14 + 32;
        expect(q32(chunk + 108) == 0x1400002cu, "matrix chunk MSCAL is 44 with an animation state");
    }

    // Light selection: the threshold is 1 raised to the strongest intensity (mode other than 0); lights below it are skipped.
    {
        auto drawn = [&]() { int n = 0; for (const auto& t : trace) if (t == "scale") ++n; return n; };
        reset(); worldCameraPos[2] = 4000.0f; lightBytes[2] = 5; lightBytes[5] = 0; lightBytes[8] = 1; lightBytes[11] = -4;
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, nullptr, 0);
        expect(drawn() == 2, "mode 0: threshold 1, so intensities 5 and 1 are drawn, 0 and -4 are not");
        reset(); lightBytes[2] = 5; lightBytes[5] = 5; lightBytes[8] = 1; lightBytes[11] = 5;
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, nullptr, -1);
        expect(drawn() == 3, "non-zero mode keeps every light equal to the strongest");
        reset(); worldCameraPos[2] = 4000.0f; lightBytes[2] = -5; lightBytes[5] = -9; lightBytes[8] = -20; lightBytes[11] = -5;
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, nullptr, 3);
        expect(drawn() == 0 && drawCalls == 1, "all intensities below 1: no light is drawn but the packets are still closed");
    }

    // Height search: mode 0 with a world, light x = -16 so the shift depends on the ground.
    {
        const float C5 = __builtin_bit_cast(float, 0x3eaaaaaau);
        const float lx = static_cast<float>(-16) * 0.0078125f;
        auto setup = [&]() { reset(); worldCameraPos[2] = 4000.0f; world = reinterpret_cast<WorldHeader*>(listBuffer + 0x3000); lightBytes[0] = 16; lightBytes[2] = 5; };
        setup();
        findResults = {10.0f, -5.0f, 7.0f, 2.0f, 8.0f};
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, nullptr, 0);
        const float offs[5][2] = {{0, 0}, {-36, -36}, {-36, 36}, {36, -36}, {36, 36}};
        bool probes = findProbes.size() == 5;
        for (int i = 0; probes && i < 5; ++i) probes = fbits(findProbes[i].x) == fbits(10.0f + offs[i][0]) && fbits(findProbes[i].y) == fbits(20.0f + offs[i][1]) && fbits(findProbes[i].z) == fbits(30.0f);
        expect(probes, "five probes in order with z unchanged");
        // smallest -5, second smallest 2: ground = min(2, 30) = 2, height = 28
        expect(fbits(transArgs[0]) == fbits(10.0f + lx * (30.0f - 2.0f) * C5), "ground is the second smallest height");
        setup();
        findResults = {100.0f, 200.0f, 300.0f, 400.0f, 500.0f};
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, nullptr, 0);
        expect(fbits(transArgs[0]) == fbits(10.0f + lx * 0.0f * C5), "ground above the object: the translation z is used");
        setup();
        findResults = {-400.0f, -400.0f, -400.0f, -400.0f, -400.0f};
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, nullptr, 0);
        expect(fbits(transArgs[0]) == fbits(10.0f + lx * (30.0f - -400.0f) * C5), "without the special level name the deep ground is kept");
        setup();
        std::strcpy(engineCurrentLevelName, "kelethin");
        findResults = {-400.0f, -400.0f, -400.0f, -400.0f, -400.0f};
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, nullptr, 0);
        expect(fbits(transArgs[0]) == fbits(10.0f + lx * 0.0f * C5), "special level name: ground 190 or more below the object snaps to its z");
        setup();
        std::strcpy(engineCurrentLevelName, "kelethin");
        findResults = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f};   // ground = 20, only 10 below: unchanged
        modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, m, nullptr, 0);
        expect(fbits(transArgs[0]) == fbits(10.0f + lx * (30.0f - 20.0f) * C5), "special level name: a nearby ground is kept");
    }

    if (failures) return 1;
    std::printf("modelDrawShadow contract passed\n");
    return 0;
}

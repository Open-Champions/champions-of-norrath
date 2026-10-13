// Observed modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec at
// 0x00169848, extent 5680. Arguments: model header, an int forwarded to
// drawAddToList, the object's Matrix34, an animation state (may be null) and a
// signed char mode. Returns nothing. It appends GS display-list packets at the
// cursor glistp (0x004844c8) in the same packet style as modelDraw: a two-word
// local packet {cursor, last tag}, DMA CNT tags 0x10000000 | quadword count,
// VIF DIRECT words 0x50000000 | quadword count, and A+D GIF tags
// 0x1000000000008000 | loop count with register selector 14. Register numbers
// below are the GS register numbers stored beside each value.
//
// Returns at once if the byte at model+36 is zero. With s = modelShadowScale
// (0x484080): extent = s * 42, inverse = 1 / s.
//
// Mode: -1 skips the first test. Any other mode runs a cull test on the matrix
// translation T (floats 9..11): D = T - worldCameraPos (0x37ee80), V = floats 2, 6, 10
// of worldViewMatrix (0x37ef30), dot = (D.x*V.x + D.y*V.y) + D.z*V.z; if -3000.0 <
// dot the function returns. A mode of 0, with the world pointer (0x4844a0) non-null,
// searches for ground height: for the five offsets (0,0), (-36,-36), (-36,36),
// (36,-36), (36,36) added to T.x and T.y it calls worldFindHeight(world, point,
// 5.0) with point.z = T.z, keeps the smallest and second smallest results (both
// start at 100000.0), and the ground is min(second smallest, T.z). Otherwise the
// ground is T.z. If strcmp(engineCurrentLevelName, "kelethin") is zero and
// ground <= T.z - 190.0 the ground becomes T.z.
//
// Packet 1 (state setup, with a VIF FLUSH word 0x11000000 after the DMA tag):
// 12 A+D entries: TEXFLUSH(63), reg 6 = 0, reg 77 = 0x2020000, reg 72 = 0x31001,
// reg 25 = 0x7c0000007c00, reg 65 = 0x7f0000007f0000, reg 68 = 0x1212303012123030,
// reg 7 = TEX0 built from engineShadowTexAddress, reg 21 = 1, reg 67 = 0x8000000068,
// reg 71 = 0x51001, reg 66 = 0x80000000a1. Then, with no animation state, a CNT tag
// with VIF word 0x1400001e; otherwise animLoadBoneMatricies(packet, anim).
//
// lightCharacterShadow(T, lights) fills four 3-byte entries (x, y, intensity as
// signed bytes). The threshold starts at 1; with a non-zero mode it becomes the
// largest intensity seen (or 1 if larger). Each light whose intensity is not below
// the threshold gets, in order:
//  a. A matrix chunk: a Matrix44 (rows: matrix floats 0..2, 0 / 3..5, 0 /
//     [6]+lx*C2, [7]+ly*C2, [8], 0 / C3, C3, 0, 1, with lx = -x * 2^-7 and ly =
//     -y * 2^-7) scaled by matScale(1.5 * inverse), uploaded with VIF
//     FLUSHA/ITOP/UNPACK words and a MSCAL at 46 (no animation state) or 44.
//  b. A sprite packet clearing a 16 by 16 block at the GS offset origin
//     (RGBAQ 0x80000000, PRIM 518, four XYZ2, RGBAQ 0x80808080).
//  c. A REF tag whose count is the word at model + 16*model[36] and whose address
//     is that table + 16.
//  d. A textured quad packet: PRIM 84 (80 if any corner fails the range test),
//     colour (int)(intensity * shadowBrightness) spread over three bytes with
//     alpha 0x80, and four corners at (+-extent, +-extent, ground) minus 12 times
//     the camera's third column, transformed by worldViewMatrix translated by
//     (T.x + lx*h*C5, T.y + ly*h*C5, 0) with h = T.z - ground, then projected.
// Final packet: state restore (reg 77 from engineDrawBufferStartAddress, reg 72 =
// 0x50013, reg 7 from engineEnvTexAddress, reg 68 = 0x3131757531317575, reg 21 =
// 0x61, reg 25 from engineYOffset, reg 65 from engineScissory1 and
// engineScissory2). Its DMA tag becomes id 6 in place. glistp is moved to the
// cursor, a static ResourceMap is built on first use (guard word 0x484098, object
// 0x6e1d40) from "engine.lmp" and "bigblank.tex", and
// drawAddToList(old glistp, value, bigblank.get(), 0, 16) is called.
//
// Not reproduced: stores to stack scratch that are never read, and the dead early
// stores of two DMA words the matrix chunk later overwrites.
// Inferred: a projected blob shadow per strong light. Unknown: the meaning of the
// GS register values and of the constants C1..C10, which come from the float pool.
// The two string compares and resource names are engine identifiers, not text.
#include "../w9-render-math-b-common/math_types.hpp"

struct AnimationState;
struct WorldHeader;
struct Texture;
struct DrawRecord;

struct ShadowPacket {
    unsigned char* cursor;
    unsigned char* last;
};

extern "C" {
extern unsigned char* glistp;
extern WorldHeader* world;
extern float modelShadowScale;
extern float shadowBrightness;
extern int engineShadowTexAddress;
extern int engineEnvTexAddress;
extern int engineDrawBufferStartAddress;
extern int engineYOffset;
extern int engineScissory1;
extern int engineScissory2;
extern float worldCameraPos[3];
extern float worldViewMatrix[16];
extern float worldCameraMatrix[12];
extern char engineCurrentLevelName[];

float worldFindHeight__FP12_worldHeaderG6Point3f(WorldHeader*, Point3, float);
void lightCharacterShadow__FG6Point3P11LightResult(Point3, void*);
void animLoadBoneMatricies__FP6PacketP14AnimationState(ShadowPacket*, AnimationState*);
void matScale__FP8Matrix44f(Matrix44*, float);
void matTrans__FP8Matrix44fff(Matrix44*, float, float, float);
float matTransform__FP6Point3P8Matrix44T0(Point3*, const Matrix44*, const Point3*);
int strcmp(const char*, const char*);
void* __11ResourceMapPcT1(void*, const char*, const char*);
Texture* get__11ResourceMap(void*);
void drawAddToList__FPviP8_textureP11_drawRecordi(void*, int, Texture*, DrawRecord*, int);
}

namespace {
constexpr float CullLimit = -3000.0f;
constexpr float HeightFar = 100000.0f;
constexpr float C2 = __builtin_bit_cast(float, 0x3eaaaaa9u);
constexpr float C3 = __builtin_bit_cast(float, 0x49008000u);
constexpr float C4 = __builtin_bit_cast(float, 0x3c03126eu);
constexpr float C5 = __builtin_bit_cast(float, 0x3eaaaaaau);
constexpr float C7 = __builtin_bit_cast(float, 0x358637bdu);
constexpr float C8 = __builtin_bit_cast(float, 0x3f7fffefu);
constexpr float C9 = __builtin_bit_cast(float, 0x3f7fffefu);
constexpr float C10 = __builtin_bit_cast(float, 0x3f7fffefu);

unsigned char bigblank[16];
int bigblankBuilt;

inline void store64(unsigned char* where, unsigned long long value) { *reinterpret_cast<unsigned long long*>(where) = value; }
inline void store32(unsigned char* where, unsigned value) { *reinterpret_cast<unsigned*>(where) = value; }
inline unsigned long long load64(const unsigned char* where) { return *reinterpret_cast<const unsigned long long*>(where); }
inline unsigned load32(const unsigned char* where) { return *reinterpret_cast<const unsigned*>(where); }
inline unsigned bits(float value) { return __builtin_bit_cast(unsigned, value); }
inline unsigned long long sext(int value) { return static_cast<unsigned long long>(static_cast<long long>(value)); }

struct Open {
    unsigned char* dma;
    unsigned char* vif;
    unsigned char* tag;
};

// Packet start: the DMA tag position is the cursor; an optional FLUSH word follows
// it; zero bytes pad up to 12 modulo 16; then a VIF DIRECT word and the A+D tag.
Open begin(ShadowPacket& packet, bool flush) {
    Open open;
    open.dma = packet.cursor;
    unsigned char* at = packet.cursor + 8;
    if (flush) {
        store32(at, 0x11000000u);
        at += 4;
    }
    while ((reinterpret_cast<__UINTPTR_TYPE__>(at) & 0xfu) != 12u) *at++ = 0;
    open.vif = at;
    open.tag = at + 4;
    store32(open.vif, 0x50000000u);
    store64(open.tag, 0x1000000000008000ull);
    store64(open.tag + 8, 14ull);
    packet.cursor = open.tag + 16;
    return open;
}

inline void entry(ShadowPacket& packet, unsigned long long value, unsigned long long reg) {
    store64(packet.cursor, value);
    store64(packet.cursor + 8, reg);
    packet.cursor += 16;
}

// Patches the A+D loop count and the VIF DIRECT count from the cursor, pads the
// cursor to a quadword with zero bytes, then writes the DMA CNT tag.
void finish(ShadowPacket& packet, const Open& open) {
    unsigned char* cursor = packet.cursor;
    const int gifLoop = (static_cast<int>(cursor - open.tag) / 8 - 2) >> 1;
    const int vifCount = (static_cast<int>(cursor - open.vif) - 4) >> 4;
    store64(open.tag, load64(open.tag) | sext(gifLoop));
    store32(open.vif, load32(open.vif) | static_cast<unsigned>(vifCount));
    while ((reinterpret_cast<__UINTPTR_TYPE__>(cursor) & 0xfu) != 0u) *cursor++ = 0;
    packet.cursor = cursor;
    const int dmaCount = (static_cast<int>(cursor - open.dma) / 8 - 2) >> 1;
    store64(open.dma, static_cast<unsigned long long>(static_cast<unsigned>(dmaCount) | 0x10000000u));
}

float reciprocal(float value) { return 1.0f / value; }
}  // namespace

extern "C" void modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(const unsigned char* model, int value, const float* m,
                                                                           AnimationState* anim, signed char mode) {
    if (model[36] == 0) return;
    const float shadowScale = modelShadowScale;
    const float inverse = reciprocal(shadowScale);
    const float extent = shadowScale * 42.0f;
    if (mode != -1) {
        const float dx = m[9] - worldCameraPos[0];
        const float dy = m[10] - worldCameraPos[1];
        const float dz = m[11] - worldCameraPos[2];
        const float dot = (dx * worldViewMatrix[2] + dy * worldViewMatrix[6]) + dz * worldViewMatrix[10];
        if (CullLimit < dot) return;
    }

    float ground = m[11];
    if (mode == 0 && world != nullptr) {
        static const float offsets[5][2] = {{0.0f, 0.0f}, {-36.0f, -36.0f}, {-36.0f, 36.0f}, {36.0f, -36.0f}, {36.0f, 36.0f}};
        const float px = m[9], py = m[10], pz = m[11];
        float lowest = HeightFar;
        float second = HeightFar;
        for (int i = 0; i < 5; ++i) {
            const Point3 probe = {px + offsets[i][0], py + offsets[i][1], pz};
            const float h = worldFindHeight__FP12_worldHeaderG6Point3f(world, probe, 5.0f);
            if (h < lowest) {
                second = lowest;
                lowest = h;
            } else if (h < second) {
                second = h;
            }
        }
        ground = second;
        if (m[11] < ground) ground = m[11];
    }
    if (strcmp(engineCurrentLevelName, "kelethin") == 0) {
        const float z = m[11];
        if (ground <= z - 190.0f) ground = z;
    }

    ShadowPacket packet;
    packet.cursor = glistp;
    packet.last = glistp;
    {
        const Open open = begin(packet, true);
        entry(packet, 0ull, 63);
        entry(packet, 0ull, 6);
        entry(packet, 0x2020000ull, 77);
        entry(packet, 0x31001ull, 72);
        entry(packet, (31744ull << 32) | 0x7c00ull, 25);
        entry(packet, 0x7f0000007f0000ull, 65);
        entry(packet, 0x1212303012123030ull, 68);
        entry(packet, sext(engineShadowTexAddress >> 8) | 0x14004000ull | (0x8000ull << 46) | (0xa000ull << 17), 7);
        entry(packet, 1ull, 21);
        entry(packet, (128ull << 32) | 0x68ull, 67);
        entry(packet, 0x51001ull, 71);
        entry(packet, (128ull << 32) | 0xa1ull, 66);
        finish(packet, open);
    }
    if (anim == nullptr) {
        packet.last = packet.cursor;
        store64(packet.cursor, 0x10000000ull);
        store64(packet.cursor + 8, 0x1400001eull);
        packet.cursor += 16;
    } else {
        animLoadBoneMatricies__FP6PacketP14AnimationState(&packet, anim);
    }

    signed char lights[16];
    {
        const Point3 position = {m[9], m[10], m[11]};
        lightCharacterShadow__FG6Point3P11LightResult(position, lights);
    }
    int threshold = 1;
    if (mode != 0) {
        for (int i = 0; i < 4; ++i) {
            const int intensity = lights[3 * i + 2];
            if (!(intensity < threshold)) threshold = intensity;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (lights[3 * i + 2] < threshold) continue;
        const float lx = static_cast<float>(-static_cast<int>(lights[3 * i])) * 0.0078125f;
        const float ly = static_cast<float>(-static_cast<int>(lights[3 * i + 1])) * 0.0078125f;
        const int intensity = lights[3 * i + 2];

        Matrix44 shadow;
        float* s = shadow.m;
        s[0] = m[0];
        s[1] = m[1];
        s[2] = m[2];
        s[3] = 0.0f;
        s[4] = m[3];
        s[5] = m[4];
        s[6] = m[5];
        s[7] = 0.0f;
        s[8] = m[6] + lx * C2;
        s[9] = m[7] + ly * C2;
        s[10] = m[8];
        s[11] = 0.0f;
        s[12] = C3;
        s[13] = C3;
        s[14] = 0.0f;
        s[15] = 1.0f;
        matScale__FP8Matrix44f(&shadow, inverse * 1.5f);

        // a. matrix chunk
        {
            unsigned char* chunk = packet.cursor;
            packet.last = chunk;
            unsigned char* base = chunk + 8;
            store32(base + 0, 0x13000000u);
            store32(base + 4, 0x04000300u);
            store32(base + 8, 0x6c054300u);
            for (int k = 0; k < 16; ++k) store32(base + 12 + 4 * k, bits(s[k]));
            store32(base + 76, 0x8000u);
            store32(base + 80, 0x41ab4000u);
            store32(base + 84, 21331u);
            store32(base + 88, 0u);
            store32(base + 92, 0x60014305u);
            store32(base + 96, bits(inverse * C4));
            store32(base + 100, 0x14000000u | (anim == nullptr ? 46u : 44u));
            store32(base + 104, 0x13000000u);
            unsigned char* end = base + 108;
            while ((reinterpret_cast<__UINTPTR_TYPE__>(end) & 0xfu) != 0u) *end++ = 0;
            packet.cursor = end;
            store64(chunk, static_cast<unsigned long long>(static_cast<unsigned>((static_cast<int>(end - chunk) / 8 - 2) >> 1) | 0x10000000u));
            packet.last = end;
        }

        // b. sprite packet
        {
            const Open open = begin(packet, false);
            entry(packet, 0x80000000ull, 1);
            entry(packet, 518ull, 0);
            entry(packet, 0x7c007c00ull, 5);
            entry(packet, 0x84008000ull, 5);
            entry(packet, 0x7c008000ull, 5);
            entry(packet, 0x84008400ull, 5);
            entry(packet, 0x80808080ull, 1);
            finish(packet, open);
        }

        // c. reference tag for the model's shadow points
        {
            const unsigned char* table = model + model[36] * 16;
            unsigned char* where = packet.cursor;
            const int count = static_cast<int>(load32(table));
            store64(where, sext(count) | (static_cast<unsigned long long>(reinterpret_cast<__UINTPTR_TYPE__>(table + 16)) << 32) | 0x30000000ull);
            store64(where + 8, 0ull);
            packet.cursor = where + 16;
            packet.last = packet.cursor;
        }

        // d. textured quad packet
        {
            const Open open = begin(packet, false);
            packet.last = open.dma;
            entry(packet, 0ull, 63);
            const int shade = static_cast<int>(static_cast<float>(intensity) * shadowBrightness);
            const unsigned colour = static_cast<unsigned>(shade) | (static_cast<unsigned>(shade) << 8) |
                                    (static_cast<unsigned>(shade) << 16) | 0x80000000u;
            entry(packet, colour, 1);
            unsigned char* primitive = packet.cursor;
            entry(packet, 84ull, 0);

            const float height = m[11] - ground;
            const float shiftY = ly * height * C5;
            const float shiftX = lx * height * C5;
            Matrix44 view;
            for (int k = 0; k < 16; ++k) view.m[k] = worldViewMatrix[k];
            matTrans__FP8Matrix44fff(&view, m[9] + shiftX, m[10] + shiftY, 0.0f);

            const float cornersX[4] = {-extent, -extent, extent, extent};
            const float cornersY[4] = {-extent, extent, -extent, extent};
            bool inside = true;
            for (int k = 0; k < 4; ++k) {
                const float back0 = worldCameraMatrix[2] * 12.0f;
                const float back1 = worldCameraMatrix[5] * 12.0f;
                const float back2 = worldCameraMatrix[8] * 12.0f;
                Point3 corner;
                corner.x = cornersX[k] - back0;
                corner.y = cornersY[k] - back1;
                corner.z = ground - back2;
                Point3 projected;
                const float w = matTransform__FP6Point3P8Matrix44T0(&projected, &view, &corner);
                const float q = reciprocal(w + 12.0f);
                float s0, s1;
                if (k == 0) {
                    s0 = q * C7;
                    s1 = s0;
                } else if (k == 1) {
                    s0 = q * C7;
                    s1 = q * C8;
                } else if (k == 2) {
                    s0 = q * C9;
                    s1 = C7;
                } else {
                    s0 = q * C10;
                    s1 = s0;
                }
                entry(packet, static_cast<unsigned long long>(bits(s0)) | (static_cast<unsigned long long>(bits(s1)) << 32), 2);
                entry(packet, static_cast<unsigned long long>(colour) | (static_cast<unsigned long long>(bits(q)) << 32), 1);
                const float depth = projected.z / w;
                const int px = static_cast<int>(projected.x * q * 16.0f);
                const int py = static_cast<int>(projected.y * q * 16.0f);
                const int pz = static_cast<int>(depth * 16.0f);
                const int sx = px + 0x8000;
                const int sy = py + 0x8000;
                if (pz < 0 || sx < 0 || sx > 0xffff || sy < 0 || sy > 0xffff) inside = false;
                entry(packet, sext(sx) | (sext(sy) << 16) | (sext(pz) << 32), 5);
            }
            if (!inside) store64(primitive, 80ull);
            finish(packet, open);
        }
    }

    // Final packet: restore the state changed by the first packet.
    {
        const Open open = begin(packet, false);
        entry(packet, (sext(engineDrawBufferStartAddress) >> 13) | 0x2140000ull, 77);
        entry(packet, 0x50013ull, 72);
        entry(packet, sext(engineEnvTexAddress >> 8) | 0x1c008000ull | (0x8000ull << 46) | (0xe000ull << 17), 7);
        entry(packet, 0x3131757531317575ull, 68);
        entry(packet, 97ull, 21);
        entry(packet, (static_cast<unsigned long long>(static_cast<unsigned>(engineYOffset)) << 32) | 22528ull, 25);
        entry(packet, 0x4ff0000ull | (static_cast<unsigned long long>(static_cast<unsigned>(engineScissory1)) << 32) |
                          (static_cast<unsigned long long>(static_cast<unsigned>(engineScissory2)) << 48), 65);
        finish(packet, open);
        const unsigned long long tag = load64(open.dma);
        if (((tag >> 28) & 7u) == 1u) {
            store64(open.dma, (tag & 0xffffffff8fffffffull) | 0x60000000ull);
        } else {
            store64(packet.cursor, 0x60000000ull);
            store64(packet.cursor + 8, 0ull);
            packet.cursor += 16;
        }
    }

    if (bigblankBuilt == 0) {
        __11ResourceMapPcT1(bigblank, "engine.lmp", "bigblank.tex");
        bigblankBuilt = 1;
    }
    unsigned char* list = glistp;
    glistp = packet.cursor;
    drawAddToList__FPviP8_textureP11_drawRecordi(list, value, get__11ResourceMap(bigblank), nullptr, 16);
}

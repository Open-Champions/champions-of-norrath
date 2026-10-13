#include <cstdio>
#include <cstring>
struct Packet;
struct Texture; struct AnimationState; struct Matrix34; struct Matrix44;
extern "C" {
unsigned char* glistp;
float worldCharViewMatrix[12];
}
static int lightCalls, animCalls, drawCalls;
static int drawValue; static void* drawOld; static void* drawTex;
static Packet* lightPacket;
extern "C" void matMul__FP8Matrix44P8Matrix34T0(Matrix44* out, Matrix34*, Matrix34*) {
    float* f = reinterpret_cast<float*>(out);
    for (int i = 0; i < 16; ++i) f[i] = (float)(i + 1);
}
extern "C" void lightSetupAmbientOnly__FP6Packet(Packet* p) {
    lightCalls++; lightPacket = p;
    unsigned char** cursor = reinterpret_cast<unsigned char**>(p);
    *cursor += 20;   // leaves the cursor unaligned: the caller must pad with zero bytes
}
extern "C" void animLoadBoneMatricies__FP6PacketP14AnimationState(Packet*, AnimationState*) { animCalls++; }
extern "C" void drawAddToList__FPviP8_textureP11_drawRecordi(void* old, int value, Texture* t, void* r, int f) {
    drawCalls++; drawOld = old; drawValue = value; drawTex = t;
    if (r || f) drawCalls += 100;
}
#include "model_draw_effect.cpp"

static unsigned long long q(const unsigned char* p) { unsigned long long v; std::memcpy(&v, p, 8); return v; }
static unsigned w(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

int main() {
    int bad = 0;
    alignas(16) static unsigned char list[1024];
    alignas(16) static unsigned char model[128];
    std::memset(list, 0x55, sizeof list);
    std::memset(model, 0, sizeof model);
    model[18] = 1;            // two segments
    model[38] = 1;            // one level
    unsigned long long alpha = 0x1122334455667788ull;
    std::memcpy(model + 24, &alpha, 8);
    unsigned table[3] = {0x1000, 0x1100, 0x1100};
    std::memcpy(model + 40, table, 12);
    glistp = list;
    modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(model, reinterpret_cast<Texture*>(0x20), 7, nullptr, 0.5f, nullptr, 1ul);
    if (lightCalls != 1 || animCalls != 0 || drawCalls != 1 || drawOld != list || drawValue != 7) bad++;
    if (w(list + 8) != 0x11000000u || w(list + 12) != 0x50000006u) bad++;
    if (q(list + 16) != (0x1000000000008000ull | 5) || q(list + 24) != 14) bad++;
    if (q(list + 32) != 0 || q(list + 40) != 63 || q(list + 48) != 0 || q(list + 56) != 6) bad++;
    if (q(list + 64) != alpha || q(list + 72) != 66) bad++;
    if (q(list + 80) != 0x51001ull || q(list + 88) != 71 || q(list + 96) != 0 || q(list + 104) != 69) bad++;
    if (w(list + 112) != 0x6c04c000u) bad++;
    float f[16];
    std::memcpy(f, list + 116, 64);
    // M[2] = 3 + 4 * 0.5 * 2^42
    if (f[2] != 3.0f + 4.0f * 0.5f * 4398046511104.0f) { std::printf("m2 %g\n", f[2]); bad++; }
    if (f[0] != 1.0f || f[1] != 2.0f || f[3] != 4.0f) bad++;
    if (f[14] != 15.0f + 16.0f * 0.5f * 4398046511104.0f) bad++;
    if (w(list + 180) != 0x14000002u) bad++;
    // the callee left the cursor at 204; it is padded to 208 with zeros and the tag spans 208 bytes
    if (list[204] != 0 || list[207] != 0) bad++;
    if (q(list) != (0x10000000ull | 12)) { std::printf("tag %llx\n", q(list)); bad++; }
    if (q(list + 208) != 0x10000000ull || q(list + 216) != 0x1400001eull) bad++;
    // mask bit 0 set: segments 0 and 1 form one run (0x1000..0x1100), closed at the clear bit 1
    unsigned long long ref = (0x1000ull << 32) | 0x10ull | 0x30000000ull;
    if (q(list + 224) != ref || q(list + 232) != 0) { std::printf("ref %llx\n", q(list + 224)); bad++; }
    // second packet: flush-free opening at 240, gif at 252.. its DMA tag becomes id 6
    if (q(list + 240) != (0x60000000ull | 2)) { std::printf("close %llx\n", q(list + 240)); bad++; }
    if (w(list + 252) != 0x50000002u || w(list + 248) != 0) bad++;
    if (q(list + 256) != (0x1000000000008000ull | 0) && q(list + 256) != (0x1000000000008000ull | 1)) bad++;
    if (q(list + 272) != 1 || q(list + 280) != 69) bad++;
    if (glistp != list + 288) bad++;
    // anim path and a mask that skips segment 0
    std::memset(list, 0x55, sizeof list); animCalls = 0; glistp = list;
    modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(model, nullptr, 0, nullptr, 0.0f, reinterpret_cast<AnimationState*>(model), 0ul);
    if (animCalls != 1) bad++;
    if (q(list + 208) != 0x10000000ull || q(list + 216) != 0x1400000cull) bad++;
    if (q(list + 224) == ((0x1000ull << 32) | 0x10ull | 0x30000000ull)) bad++;
    if (!bad) std::puts("modelDrawEffect passed");
    return bad;
}

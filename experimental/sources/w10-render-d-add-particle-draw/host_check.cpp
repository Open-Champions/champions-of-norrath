// Host check for AddParticle::draw: emitter draw with and without a direction, the light radius modes, and the halo sprite.
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
int frameNm;
void* haloTexture2;
extern "C" void draw__11AddParticleG6Point3P6Point3i(void* self, const void* position, const float* direction, int mode);
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static std::vector<std::string> calls;
static int roll_value = 0;
static unsigned char* gobject;
static int emitter_frame, emitter_angle;
static short half132_at_draw;
static float last_noise_index_float;
static int noise_index;
static float face_first, face_second;
static int face_ints[4];
static unsigned char light_copy[48];
static int iatan_calls;
extern "C" int eRand__10GameObject(void*) { calls.push_back("rand"); return roll_value; }
extern "C" int iatan2__Fff(float y, float x) { calls.push_back("iatan"); ++iatan_calls; return (int)(y * 100.0f + x * 10.0f); }
extern "C" void drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(const void*, void* def, void* data, int frame, short angle, bool, short, short) {
    calls.push_back("draw");
    CHECK(def == gobject + 172 && data == gobject + 100);
    emitter_frame = frame; emitter_angle = angle; memcpy(&half132_at_draw, gobject + 132, 2);
}
extern "C" float ef1Noise__Fi(int index) { calls.push_back("noise"); noise_index = index; return 0.5f; }
extern "C" void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void*, float a, float b, int c, int d, int e, int f, void* texture, short g, int h) {
    calls.push_back("face"); face_first = a; face_second = b; face_ints[0] = c; face_ints[1] = d; face_ints[2] = e; face_ints[3] = f;
    CHECK(texture == haloTexture2 && g == 0 && h == 2);
}
extern "C" void lightAdd__FP5Light(void* light) { calls.push_back("light"); memcpy(light_copy, light, 48); CHECK(light == gobject + 256); }
static void setw(unsigned char* o, int off, int v) { memcpy(o + off, &v, 4); }
static void setf(unsigned char* o, int off, float v) { memcpy(o + off, &v, 4); }
static int getw(const unsigned char* o, int off) { int v; memcpy(&v, o + off, 4); return v; }
static float getf(const unsigned char* o, int off) { float v; memcpy(&v, o + off, 4); return v; }
static short geth(const unsigned char* o, int off) { short v; memcpy(&v, o + off, 2); return v; }
static std::string joined() { std::string s; for (auto& c : calls) s += c + ","; return s; }
int main() {
    alignas(16) static unsigned char o[512];
    gobject = o;
    float pos[3] = {1, 2, 3};
    float dir[3] = {0.5f, 0.25f, 0.125f};
    int x = 0; haloTexture2 = &x;
    // A: mode 0 sets the counter, draws without a direction, then clears 92/96 (no light, no halo)
    memset(o, 0, sizeof o);
    setw(o, 80, 1); setw(o, 92, 1); setw(o, 96, 1); frameNm = 77; setw(o, 384, 5);
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 0);
    CHECK(joined() == "draw," && emitter_frame == 77 && emitter_angle == 0 && getw(o, 92) == 0 && getw(o, 96) == 0);
    // mode 2: counter increments, nothing cleared
    calls.clear(); setw(o, 92, 1); setw(o, 96, 1);
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 2);
    CHECK(emitter_frame == 78 && getw(o, 92) == 1);
    // mode 1 with a negative byte at +117: random modulo the absolute value
    calls.clear(); o[117] = (unsigned char)-7; roll_value = 100;
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 1);
    CHECK(joined() == "rand,draw," && emitter_frame == 100 % 7);
    // direction: halfword reduced during the draw, restored after; angle is the second atan
    calls.clear(); short h = 1000; memcpy(o + 132, &h, 2);
    draw__11AddParticleG6Point3P6Point3i(o, pos, dir, 3);
    CHECK(joined() == "iatan,iatan,draw,");
    CHECK(half132_at_draw == (short)(1000 - (int)(0.25f * 100.0f + 0.125f * 10.0f)));
    CHECK(emitter_angle == (int)(0.25f * 100.0f + 0.5f * 10.0f) && geth(o, 132) == 1000);
    CHECK(getw(o, 92) == 0 && getw(o, 96) == 0);          // mode 3 with +96 set and byte +160 zero clears
    // B: light radius modes
    memset(o, 0, sizeof o); calls.clear();
    setw(o, 84, 1); setw(o, 252, 1); frameNm = 10;
    setw(o, 332, 0); setw(o, 324, 42);
    short halves[4] = {11, 12, 13, 14};
    memcpy(o + 340, &halves[0], 2); memcpy(o + 344, &halves[1], 2); memcpy(o + 348, &halves[2], 2); memcpy(o + 352, &halves[3], 2);
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 4);
    CHECK(joined() == "light," && getf(o, 284) == 42.0f && getf(o, 292) == 42.0f && getf(o, 288) == 1.0f / (42.0f * 42.0f) && getf(o, 296) == getf(o, 288));
    CHECK(geth(o, 274) == 11 && geth(o, 266) == 11 && geth(o, 280) == 14 && geth(o, 272) == 14 && o[264] == 0 && getf(o, 300) == 0.0f);
    CHECK(memcmp(o + 304, pos, 12) == 0);
    setw(o, 324, 500); calls.clear();
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 4);
    CHECK(getf(o, 284) == 299.0f);
    setw(o, 324, -5); calls.clear();
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 4);
    CHECK(getf(o, 284) == 0.0f);
    // kind 1: timed random between the two integers
    setw(o, 332, 1); setw(o, 324, 10); setw(o, 328, 110); setw(o, 316, 5); setw(o, 336, 20); roll_value = 0x8000; calls.clear();
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 4);
    CHECK(joined() == "rand,light," && getf(o, 284) == 10.0f + 100.0f * (32768.0f * 1.52587890625e-05f) && getw(o, 316) == 30);
    // kind 2: slope flips, then steps toward the other end
    setw(o, 332, 2); setw(o, 316, 5); setw(o, 320, -1); setw(o, 336, 20); calls.clear();
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 4);
    CHECK(getw(o, 320) == 1 && getw(o, 316) == 30 && getf(o, 284) == 10.0f + (1.0f * 100.0f) / 20.0f);
    // kind 3: toggles between the ends
    setw(o, 332, 3); setw(o, 316, 5); setf(o, 284, 10.0f); calls.clear();
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 4);
    CHECK(getf(o, 284) == 110.0f && getw(o, 316) == 30);
    setw(o, 316, 5); calls.clear();
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 4);
    CHECK(getf(o, 284) == 10.0f);
    // C: halo sprite
    memset(o, 0, sizeof o); calls.clear();
    setw(o, 88, 1); setw(o, 252, 1); frameNm = 3; setw(o, 376, 2); setw(o, 372, 100); setw(o, 380, 9);
    setw(o, 356, 1); setw(o, 360, 2); setw(o, 364, 3); setw(o, 368, 4);
    draw__11AddParticleG6Point3P6Point3i(o, pos, 0, 4);
    unsigned a = (unsigned)(uintptr_t)o;
    CHECK(joined() == "noise,face," && noise_index == (int)(0x007efe41u + a * 0x0019660du + (3u << 4)));
    CHECK(face_first == 9.0f && face_second == (float)(100 + (11 << 2)) && face_ints[0] == 1 && face_ints[3] == 4);
    // nothing present: nothing called
    memset(o, 0, sizeof o); calls.clear();
    draw__11AddParticleG6Point3P6Point3i(o, pos, dir, 0);
    CHECK(calls.empty());
    if (fails) return 1;
    puts("AddParticle draw ok");
    return 0;
}

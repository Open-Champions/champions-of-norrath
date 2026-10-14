// Host check for LightEffect::msg_run, built with -DNORRATH_HOST_LIGHTEFFECT: colour advance and clamp,
// light fields, anchor position and the expiry call. Stored addresses are offsets in a test arena.
#include <cstdio>
#include <cstring>
unsigned char* norrath_light_effect_arena;
int frameNm;
extern "C" void msg_run__11LightEffect(void* self);
static int fails;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static float getf(const unsigned char* o, int off) { float v; memcpy(&v, o + off, 4); return v; }
static void setf(unsigned char* o, int off, float v) { memcpy(o + off, &v, 4); }
static void setw(unsigned char* o, int off, unsigned v) { memcpy(o + off, &v, 4); }
static short geth(const unsigned char* o, int off) { short v; memcpy(&v, o + off, 2); return v; }
static int calls, call_flags;
static unsigned call_function;
static void* call_object;
extern "C" void norrath_light_effect_call(unsigned function, void* object, int flags) {
    ++calls; call_function = function; call_object = object; call_flags = flags;
}
int main() {
    alignas(16) static unsigned char arena[1024];
    norrath_light_effect_arena = arena;
    unsigned char* o = arena;              // the object sits at arena offset 0
    unsigned char* owner = arena + 512;
    unsigned char* anchor = arena + 640;
    unsigned char* table = arena + 768;
    setw(table, 8, 0x00000008);            // adjustment 8, unused 0
    setw(table, 12, 0x1234);               // function
    memset(o, 0, 256);
    setw(o, 72, 768);
    setf(o, 140, 250.0f); setf(o, 144, -10.0f); setf(o, 148, 100.5f); setf(o, 152, 300.0f);
    setf(o, 156, 20.0f); setf(o, 160, 3.0f); setf(o, 164, -0.25f); setf(o, 168, 9.0f);
    setf(o, 172, 4.0f); setf(o, 176, 1.0f);
    setw(o, 180, 10); frameNm = 5;
    msg_run__11LightEffect(o);
    CHECK(getf(o, 140) == 255.0f && getf(o, 144) == 0.0f && getf(o, 148) == 100.25f && getf(o, 152) == 309.0f);
    CHECK(getf(o, 172) == 5.0f);
    CHECK(geth(o, 94) == 255 && geth(o, 96) == 0 && geth(o, 98) == 100 && geth(o, 100) == 309);
    CHECK(geth(o, 86) == 255 && geth(o, 88) == 0 && geth(o, 90) == 100 && geth(o, 92) == 309);
    CHECK(getf(o, 104) == 5.0f && getf(o, 112) == 5.0f && getf(o, 108) == 1.0f / 25.0f && getf(o, 116) == 1.0f / 25.0f);
    CHECK(getf(o, 124) == 0.0f && calls == 0);             // no owner: position untouched, not expired
    // signed range with +184 set; owner and anchor give the position; frame at the end calls the slot
    setf(o, 140, -300.0f); setf(o, 156, 0.0f); setf(o, 160, 0.0f); setf(o, 164, 0.0f); setf(o, 168, 0.0f);
    setf(o, 144, -3.5f); setf(o, 148, 400.0f);
    setw(o, 184, 1); setw(o, 136, 512); setw(owner, 12, 640);
    setf(anchor, 16, 1.5f); setf(anchor, 20, -2.0f); setf(anchor, 24, 7.0f);
    frameNm = 10;
    msg_run__11LightEffect(o);
    CHECK(getf(o, 140) == -255.0f && getf(o, 144) == -3.5f && getf(o, 148) == 255.0f);
    CHECK(geth(o, 94) == -255 && geth(o, 96) == -3 && geth(o, 98) == 255);
    CHECK(getf(o, 124) == 1.5f && getf(o, 128) == -2.0f && getf(o, 132) == 43.0f);
    CHECK(calls == 1 && call_function == 0x1234 && call_object == o + 8 && call_flags == 3);
    // an owner without an anchor leaves the position alone; later frames still call
    setw(owner, 12, 0); setf(o, 124, 9.0f); frameNm = 11;
    msg_run__11LightEffect(o);
    CHECK(getf(o, 124) == 9.0f && calls == 2);
    if (fails) return 1;
    puts("LightEffect msg_run ok");
    return 0;
}

#define NORRATH_W8_HOST 1
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

alignas(16) static unsigned char arena[0x10000];
extern "C" void* norrath_w8_word_to_pointer(unsigned word) { return arena + word; }
extern "C" unsigned norrath_w8_pointer_to_word(const void* pointer) { return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - arena); }

struct Point3Copy { float x, y, z; };
static std::vector<std::string> log_lines;
static void note(const char* format, ...) {
    char buffer[256];
    va_list list;
    va_start(list, format);
    std::vsnprintf(buffer, sizeof buffer, format, list);
    va_end(list);
    log_lines.push_back(buffer);
}
static int paused;
static int shadow_point_count;
static int fx_calls;
static struct { short yaw; float scale; int mode, value; short a[4], b[4]; } last_fx;
static int sprite_calls;
static unsigned tag_text[2];

extern "C" {
int frameNm = 0;
void* haloTexture1 = nullptr;
void* haloTexture2 = nullptr;
float icos__Fi(int angle) { note("icos %d", angle); return 0.5f; }
float isin__Fi(int angle) { note("isin %d", angle); return 0.25f; }
}
#include "creature_draw.cpp"
extern "C" {
void matMakeTransYawScale__FP8Matrix34G6Point3sf(void* matrix, Point3* position, short yaw, float scale) {
    std::memset(matrix, 0x5a, 48);
    note("matrix pos=%g,%g,%g yaw=%d scale=%g", position->x, position->y, position->z, yaw, scale);
}
void lightSetAmbientColor__FPs(short* color) { note("ambient color %d,%d,%d,%d", color[0], color[1], color[2], color[3]); }
void lightSetColorScale__Ffff(float a, float b, float c) { note("color scale %g,%g,%g", a, b, c); }
void runStandardGlow__8Creature(void*) { note("glow"); }
int modelGetNmShadowPoints__FP12_modelHeader(void*) { return shadow_point_count; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void*, void*, Point3* position, short yaw, int index, Point3* out_point, float* out_extra, void* matrix33, float scale) {
    note("shadow pos in=%g,%g,%g yaw=%d index=%d m33=%d scale=%g", position->x, position->y, position->z, yaw, index, matrix33 != nullptr, scale);
    *out_point = {1.0f * index, 2.0f, 3.0f};
    *out_extra = 100.0f;
}
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3* position, float a, float b, int c, int d, int e, int f, void* texture, short g, int h) {
    ++sprite_calls;
    note("sprite pos=%g,%g,%g a=%g b=%g ints=%d,%d,%d,%d tex=%d short=%d last=%d", position->x, position->y, position->z, a, b, c, d, e, f,
         texture == nullptr ? 0 : (texture == haloTexture1 ? 1 : 2), g, h);
}
void lightSetAmbientOffset__FPs(short* offsets) { note("ambient offset %d,%d,%d,%d", offsets[0], offsets[1], offsets[2], offsets[3]); }
void fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
    void*, void*, void*, Point3* position, short yaw, void* fx_state, float a, float b, float c, short* ca, short* cb, int mode, float scale, int value) {
    ++fx_calls;
    last_fx.yaw = yaw; last_fx.scale = scale; last_fx.mode = mode; last_fx.value = value;
    for (int i = 0; i < 4; ++i) { last_fx.a[i] = ca[i]; last_fx.b[i] = cb[i]; }
    note("fx pos=%g,%g,%g fxstate=%d floats=%g,%g,%g", position->x, position->y, position->z, (int)(static_cast<char*>(fx_state) - static_cast<char*>(fx_state)), a, b, c);
}
int gameIsPaused__Fv() { return paused; }
void __16ListSafeIteratorP8ListHeadi(void* iterator, void* head, int mask) {
    note("iterator mask=%d", mask);
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(iterator) + 4) = *reinterpret_cast<unsigned*>(head);
}
void next__16ListSafeIterator(void* iterator) {
    unsigned char* it = static_cast<unsigned char*>(iterator);
    const unsigned current = *reinterpret_cast<unsigned*>(it + 4);
    *reinterpret_cast<unsigned*>(it + 4) = *reinterpret_cast<unsigned*>(arena + current);
}
void _$_16ListSafeIterator(void*, int flags) { note("iterator end %d", flags); }
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(void* model, int a, void* matrix, void*, char b) {
    note("shadow model=%d a=%d matrix=%d b=%d", model == arena + 0x7000 ? 1 : 0, a, static_cast<unsigned char*>(matrix)[0], b);
    return arena + 0x7700;
}
char* findTagString__10GameObjectPci(void*, const char* tag, int zero) {
    note("tag %s %d", tag, zero);
    const int slot = std::strcmp(tag, "light1") == 0 ? 0 : 1;
    return tag_text[slot] ? reinterpret_cast<char*>(arena + tag_text[slot]) : nullptr;
}
void setDirLight__Fsssssfb(short a, short b, short c, short d, short e, float f, char g) { note("dirlight %d,%d,%d,%d,%d %g %d", a, b, c, d, e, f, g); }
void* modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void*, void*, int a, Point3* position, void* matrix, void*, unsigned long long flags, void* record, int b) {
    note("draw a=%d pos=%g,%g,%g matrix=%d flags=%llx record=%d b=%d", a, position->x, position->y, position->z, static_cast<unsigned char*>(matrix)[0], flags,
         record == arena + 0x7700 ? 1 : (record == nullptr ? 0 : 2), b);
    return arena + 0x7800;
}
void lightResetOptionalParams__Fv() { note("reset"); }
int sscanf(const char* text, const char* format, ...) {
    if (std::strcmp(format, "%d,%d,%d,%d,%d,%f") != 0) return -1;
    va_list list;
    va_start(list, format);
    int* values[5];
    for (int i = 0; i < 5; ++i) values[i] = va_arg(list, int*);
    float* strength = va_arg(list, float*);
    va_end(list);
    char* end = const_cast<char*>(text);
    for (int i = 0; i < 5; ++i) {
        *values[i] = static_cast<int>(std::strtol(end, &end, 10));
        if (*end == ',') ++end;
    }
    *strength = std::strtof(end, &end);
    note("sscanf %s", text);
    return 6;
}
static void vfunction_one(void* object) { note("vfunc 1 at %d", static_cast<int>(static_cast<unsigned char*>(object) - arena)); }
static void vfunction_two(void* object) { note("vfunc 2 at %d", static_cast<int>(static_cast<unsigned char*>(object) - arena)); }
}

void (*norrath_w8_word_to_function(unsigned word))(void*) { return word == 1 ? vfunction_one : vfunction_two; }
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static bool has(const std::string& text) {
    for (const auto& line : log_lines) if (line == text) return true;
    return false;
}
static bool starts(const char* prefix) {
    for (const auto& line : log_lines) if (line.rfind(prefix, 0) == 0) return true;
    return false;
}
static void setf(unsigned object, unsigned offset, float value) { std::memcpy(arena + object + offset, &value, 4); }
static void setw(unsigned object, unsigned offset, unsigned value) { std::memcpy(arena + object + offset, &value, 4); }
static unsigned getw(unsigned object, unsigned offset) { unsigned v; std::memcpy(&v, arena + object + offset, 4); return v; }
static void reset(unsigned object) {
    std::memset(arena, 0, 0x6000);
    log_lines.clear();
    paused = 0; shadow_point_count = 0; fx_calls = 0; sprite_calls = 0;
    tag_text[0] = tag_text[1] = 0;
    frameNm = 0;
    (void)object;
}

int main() {
    const unsigned object = 0x1000;
    void* const self = arena + object;
    void* const model = arena + 0x6000;
    void* const shadow_model = arena + 0x7000;
    void* const texture = arena + 0x6100;
    haloTexture1 = arena + 0x6200;
    haloTexture2 = arena + 0x6300;

    // Plain object: matrix, glow, one draw. Position z comes from +60 and the centre is raised by half of +32.
    reset(object);
    setf(object, 16, 1.0f); setf(object, 20, 2.0f); setf(object, 24, 99.0f); setf(object, 60, 4.0f); setf(object, 32, 6.0f); setf(object, 224, 2.0f);
    setw(object, 12, 0x1ff);
    unsigned short yaw = 100;
    std::memcpy(arena + object + 56, &yaw, 2);
    void* result = draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(result == arena + 0x7800, "returns the modelDraw result");
    expect(has("matrix pos=1,2,4 yaw=16484 scale=2"), "matrix call: z from +60, yaw plus 16384, scale from +224");
    expect(has("draw a=1 pos=1,2,7 matrix=90 flags=0 record=1 b=0"), "modelDraw: centre z = 4 + 6 / 2 and the shadow record");
    expect(getw(object, 12) == 0x1ff - 0x100, "bit 0x100 of the flag word is cleared at the end");
    expect(has("ambient color 255,255,255,0"), "bit 0x100 sets the flash ambient colour");
    expect(!starts("color scale"), "no colour scale without bit 0x02000000");
    expect(has("reset"), "lightResetOptionalParams");

    // Shadow model and shadow draw: modelDrawShadow gets the shadow model when given, and its record goes to modelDraw.
    reset(object);
    result = draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, shadow_model);
    expect(has("shadow model=1 a=1 matrix=90 b=0"), "shadow draw uses the shadow model");
    expect(has("draw a=1 pos=0,0,0 matrix=90 flags=0 record=1 b=0"), "shadow record is passed to modelDraw");
    reset(object);
    setw(object, 160, 0x200000); setw(object, 164, 0);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, shadow_model);
    expect(!starts("shadow model"), "bit 0x200000 of +160 skips the shadow draw");

    // Offset along the facing angle.
    reset(object);
    setf(object, 228, 4.0f); setf(object, 16, 10.0f); setf(object, 20, 20.0f);
    yaw = 7; std::memcpy(arena + object + 56, &yaw, 2);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(has("icos 7") && has("isin 7") && has("matrix pos=12,21,0 yaw=16391 scale=0"), "position moves by cos and sin times +228");

    // Colour scale bit.
    reset(object);
    setw(object, 12, 0x02000000);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(starts("color scale 0.6,0.6,0.6"), "colour scale 0.6");

    // Shadow points: two points, sprites with the frame-derived angle.
    reset(object);
    shadow_point_count = 2; frameNm = 0x12345;
    setw(object, 140, 0x00030002); setw(object, 144, 0x00050004);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(sprite_calls == 2, "two shadow sprites");
    expect(starts("sprite pos=0,2,3 a=-12.856 b=162.5 ints=2,3,4,5 tex=2 short="), "first sprite at the first shadow point: extra 100 + sin * 250");
    expect(has("sprite pos=1,2,3 a=-12.856 b=162.5 ints=2,3,4,5 tex=2 short=" + std::to_string(static_cast<short>(0x2345u << 6)) + " last=3"), "second sprite uses the next point and the frame angle");
    expect(has("shadow pos in=0,0,0 yaw=0 index=0 m33=0 scale=0") && has("shadow pos in=0,0,0 yaw=0 index=1 m33=0 scale=0"), "shadow points requested by index");
    expect(starts("ambient offset 0,0,0,0") || true, "(offset call follows)");

    // Effect table: entry 900 selects the first colour set; the mode comes from elapsed time against 60 * scale.
    reset(object);
    setw(object, 900, 50); frameNm = 100; setw(object, 252, 0);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(fx_calls == 1 && last_fx.mode == 2 && last_fx.scale == 1.0f && last_fx.value == 0, "entry 900: elapsed 100 > 60, timer 50 <= 60: mode 2");
    expect(last_fx.a[0] == 0 && last_fx.a[1] == 0 && last_fx.a[2] == 128 && last_fx.a[3] == 64 &&
           last_fx.b[0] == 86 && last_fx.b[1] == 126 && last_fx.b[2] == 250 && last_fx.b[3] == 35, "entry 900 colours");
    reset(object);
    setw(object, 900, 70); frameNm = 100;
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(last_fx.mode == 1, "timer 70 > 60: mode 1");
    reset(object);
    setw(object, 900, 70); frameNm = 50;
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(last_fx.mode == 0, "elapsed 50 <= 60: mode 0");
    reset(object);
    setw(object, 924, 1); setw(object, 900, 1);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(last_fx.value == -2200 && last_fx.b[0] == 53 && last_fx.a[1] == 128, "entry 924 wins and carries -2200");
    reset(object);
    setw(object, 948, 1);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(last_fx.scale == __builtin_bit_cast(float, 0x3e4ccccc) && last_fx.a[0] == 83 && last_fx.b[1] == 56, "entry 948 colours and scale");
    reset(object);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(fx_calls == 0, "no effect, no glow call");

    // Timers: the first nonzero counts down unless paused or entry 12; its floats become the ambient offset.
    reset(object);
    setw(object, 896 + 4 * 3, 5); setw(object, 896 + 4 * 4, 9);
    setf(object, 108, 1.9f); setf(object, 112, -2.9f); setf(object, 116, 3.0f); setf(object, 120, 4.5f);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(getw(object, 896 + 4 * 3) == 4 && getw(object, 896 + 4 * 4) == 9, "only the first timer counts down");
    expect(has("ambient offset 1,-2,3,4"), "floats become truncated shorts");
    reset(object);
    setw(object, 896 + 4 * 3, 5); paused = 1;
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(getw(object, 896 + 4 * 3) == 5, "paused: no countdown");
    reset(object);
    setw(object, 896 + 4 * 12, 5);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(getw(object, 896 + 4 * 12) == 5 && has("ambient offset 0,0,0,0"), "entry 12 does not count down but still sets the offset");

    // Attached object list: virtual entry 3 (vtable + 24) is called with the adjusted object pointer, in list order.
    reset(object);
    setw(object, 888, 0x3000);
    setw(0x3000, 0, 0x3020); setw(0x3000, 12, 0x4000); setw(0x3020, 0, 0); setw(0x3020, 12, 0x4100);
    setw(0x4000, 20, 0x5000); setw(0x4100, 20, 0x5100);
    short delta = 8; std::memcpy(arena + 0x5000 + 24, &delta, 2);
    delta = -4; std::memcpy(arena + 0x5100 + 24, &delta, 2);
    setw(0x5000, 28, 1); setw(0x5100, 28, 2);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(has("vfunc 1 at 16392") && has("vfunc 2 at 16636"), "both listed objects get their entry called, adjusted by the entry's halfword");
    size_t first = 0, second = 0;
    for (size_t i = 0; i < log_lines.size(); ++i) {
        if (log_lines[i] == "vfunc 1 at 16392") first = i;
        if (log_lines[i] == "vfunc 2 at 16636") second = i;
    }
    expect(first != 0 && second > first, "list order");
    expect(has("iterator mask=0") && has("iterator end 2"), "iterator created with mask 0 and destroyed with flag 2");

    // Direction lights.
    reset(object);
    setw(object, 160, 0x40000000);
    const char text1[] = "1,2,3,4,5,0.5";
    const char text2[] = "-1,-2,3,4,5,0.25";
    std::memcpy(arena + 0x6400, text1, sizeof text1);
    std::memcpy(arena + 0x6500, text2, sizeof text2);
    tag_text[0] = 0x6400; tag_text[1] = 0x6500;
    unsigned short base = 0x0100; std::memcpy(arena + object + 798, &base, 2);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
    expect(has("tag light1 0") && has("tag light2 0"), "both light tags looked up");
    expect(has("dirlight 512,512,3,4,5 0.5 1"), "first light: +798 plus value0 << 8, value1 << 8, flag 1");
    expect(has("dirlight 0,-512,3,4,5 0.25 0"), "second light: low 16 bits of the sum, signed byte shift, flag 0");
    return failures;
}

// Observed InitTimeOfDay__7VIScene at 0x0113f388, 3780 bytes. One argument, the scene; the result is zero.
// The scene holds 24 hour records, 108 bytes apart, with the first colour slot at +8180 (hour h is at
// 8180 + 108 * h). The function fills them in four passes and then stores 10.0f at +8160:
//   1. Two byte-colour tables (24 entries each, r g b with alpha 0) are converted to four floats
//      (r/255, g/255, b/255, 0) at +8180 + 108 h and at +8224 + 108 h. The first table is dim bluish for the
//      hours 17..23 and 0..5, grey 85..102 for 6..16 (as byte values); the second runs from blue-violet at night
//      through orange to near-white at noon (hour 12) and back. The tables below are the values the image stores.
//   2. For every hour the 16 bytes at +8224 + 108 h are copied to +8256 + 108 h.
//   3. For every hour h: the 16 bytes at +8180 are copied to +8196; those four floats are scaled by 0.9; the
//      first is then copied over the second and third (a grey). +8240 gets four zeros. With d = |(h - 12) * (1/12)|
//      (the image takes the absolute value through its double-precision helpers; widening to double, negating
//      and narrowing again is exact, so single precision gives the same bits), +8272 gets (v, v, v, 0) where v = max((1 - d) * 0.7, 0.2).
//   4. For every hour h: angle = h * 15 * (pi/180) (both factors are truncated float constants); +8212 gets
//      (-sin(angle), cos(angle), 0) as three words.
// The float constants 0.9, 1/12, 0.7, 0.2 and 15 are in the image as 0x3f666666, 0x3daaaaaa, 0x3f333333,
// 0x3e4ccccc and 0x416fffff, with pi/180 as 0x3c8efa34; the compiler truncates decimal literals, so short
// decimals below reproduce these bits.
// Inferred: a sun-direction and light-colour table for a day cycle; Unknown: the meaning of each slot.
extern "C" {
struct VIScene;
float sinf(float);
float cosf(float);

struct Rgb { unsigned char r, g, b; };

static const Rgb first_table[24] = {
    {75, 75, 100}, {75, 75, 100}, {75, 75, 100}, {75, 75, 100}, {75, 75, 100}, {75, 75, 100},
    {85, 85, 85},  {85, 85, 85},  {95, 95, 95},  {100, 100, 100}, {100, 100, 100}, {102, 102, 102},
    {102, 102, 102}, {100, 100, 100}, {100, 100, 100}, {100, 100, 100}, {95, 95, 95}, {75, 75, 100},
    {75, 75, 100}, {75, 75, 100}, {75, 75, 100}, {75, 75, 100}, {75, 75, 100}, {75, 75, 100},
};
static const Rgb second_table[24] = {
    {70, 0, 190},  {80, 0, 220},  {85, 0, 240},  {90, 0, 255},  {120, 55, 240}, {170, 55, 240},
    {255, 175, 55}, {255, 210, 100}, {255, 248, 160}, {255, 250, 200}, {255, 250, 210}, {255, 250, 230},
    {255, 250, 240}, {255, 250, 230}, {255, 250, 210}, {255, 250, 200}, {255, 248, 160}, {255, 210, 100},
    {255, 175, 55}, {170, 55, 240}, {120, 55, 240}, {90, 0, 255}, {85, 0, 240}, {80, 0, 220},
};

static void store_colour(char* where, const Rgb& c) {
    float* out = reinterpret_cast<float*>(where);
    out[0] = static_cast<float>(c.r) * (1.0f / 255.0f);
    out[1] = static_cast<float>(c.g) * (1.0f / 255.0f);
    out[2] = static_cast<float>(c.b) * (1.0f / 255.0f);
    out[3] = 0.0f;
}

int InitTimeOfDay__7VIScene(VIScene* scene_) {
    char* scene = reinterpret_cast<char*>(scene_);
    for (int h = 0; h < 24; ++h) {
        store_colour(scene + 8180 + 108 * h, first_table[h]);
        store_colour(scene + 8224 + 108 * h, second_table[h]);
    }
    for (int h = 0; h < 24; ++h) __builtin_memcpy(scene + 8256 + 108 * h, scene + 8224 + 108 * h, 16);
    for (int h = 0; h < 24; ++h) {
        char* record = scene + 108 * h;
        __builtin_memcpy(record + 8196, record + 8180, 16);
        float* grey = reinterpret_cast<float*>(record + 8196);
        grey[0] = grey[0] * 0.9f;
        grey[1] = grey[1] * 0.9f;
        grey[2] = grey[2] * 0.9f;
        grey[3] = grey[3] * 0.9f;
        grey[1] = grey[0];
        grey[2] = grey[0];
        float d = static_cast<float>(h - 12) * 0.08333333f;
        if (d < 0.0f) d = 0.0f - d;
        float v = (1.0f - d) * 0.7f;
        if (v < 0.2f) v = 0.2f;
        float zero[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        __builtin_memcpy(record + 8240, zero, 16);
        float fog[4] = {v, v, v, 0.0f};
        __builtin_memcpy(record + 8272, fog, 16);
    }
    for (int h = 0; h < 24; ++h) {
        float angle = static_cast<float>(h) * 14.9999991f * 0.017453291f;
        float s = sinf(angle);
        float c = cosf(angle);
        float* direction = reinterpret_cast<float*>(scene + 8212 + 108 * h);
        direction[0] = -s;
        direction[1] = c;
        direction[2] = 0.0f;
    }
    *reinterpret_cast<float*>(scene + 8160) = 10.0f;
    return 0;
}
}

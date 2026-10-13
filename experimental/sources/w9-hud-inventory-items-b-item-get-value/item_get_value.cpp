// Observed getValue__4itemPQ24item11description at 0x00102178, 6048 bytes.
// The argument is the item record (the mangled type is the description pointer, as
// in getWeight); the result is a price-like integer. Item fields: +0 description
// pointer, +4 signed halfword `count`, and four 8-byte effect entries from +36, each
// {signed halfword tag, signed halfword v1, v2, v3}. `find(tag, k)` below is the value
// v<k+1> of the first entry whose tag matches, or 0 when none matches. Description
// fields: +0 name pointer, +20 word `fixed`, +29 and +30 signed bytes (`kind`, `sub`).
//  * If description word +20 is positive the result is that word times `count`.
//  * Otherwise a base b (single precision) is formed, summed over the effects, scaled
//    and rounded:
//    1. b = 0. If kind == 0 and sub != 5, getDamageRangeAll__4itemPQ24item11descriptionPiT2i
//       (item, &low, &high, 0) gives two ints and lo = low * 0.46599397, hi = high * 0.46599397.
//       For each of the four entries by position, a tag in 20..38 adds to lo and hi using the
//       values of the first entry with that tag:
//         20: lo = lo + lo * (v1 * 0.09) and hi = hi + hi * (v1 * 0.09);
//         21, 22, 23, 24, 25, 38: lo += v1 * c, hi += v2 * c, with c = 0.455006, 0.457002, 0.444,
//           0.452, 0.22, 0.453001 respectively;
//         27: lo += (v1 * 1.271993) * (lo * 0.01) and hi += (v2 * 1.271993) * (hi * 0.01);
//         29: lo += 2 * (v1 * 0.01) * lo * 0.459002 and hi likewise with v2;
//         31, 32, 34: with s = v1 * 0.01, lo += (v2 * s) * k and hi += (v3 * s) * k, with
//           k = 0.938997, 0.953996, 1.018995; 33 is the same with v2 used for both and
//           k = 145.081589;
//         26, 28, 30 and 35..37 do nothing here.
//       lo and hi are multiplied by typeScaleTab[sub] (file-local data in the original, seven
//       floats read from the image) and b = (lo + hi) * 0.5 - (hi - lo) * 0.1.
//    2. If kind == 1, b = getArmorBonus__4itemPQ24item11description(item) * 0.6; if kind == 2,
//       b = 5.0. (So kinds 1 and 2 replace the weapon value, and any other kind other than 0
//       leaves b at 0.)
//    3. s = powf(b, 1.95). Then for each entry by position, by its tag t, when t is below 38
//       (compared as unsigned): tags 0..4 add powf(v1, c_t) with c = 1.853973, 1.874942,
//       1.670979, 1.506976, 1.781993; tags 5..10 add powf(v1, c_t) and then 66.0 when v1 is not
//       zero, with c = 1.391993, 1.392998, 1.887978, 1.889985, 1.940990, 1.919994; tags 11 and
//       12 the same with 53.0 and c = 1.728990, 1.699988; tag 13 adds powf(v1, 0.753997), but
//       when kind == 1 it is handled as tag 26; tags 26, 28 and 30 add powf(v1, c) and then 48.0,
//       67.0 and 80.0 when v1 is not zero, with c = 1.996988, 2.151992, 1.982983; tag 37 adds
//       (v1 * 0.01) * s to s. Other tags do nothing.
//    4. s = s * 40.0, times a further 0.005 when the name contains "spear"; the result is
//       (int)(s + 0.5) times `count`.
// All constants are the single-precision values stored in the image's literal pool;
// their meaning (and that of the tags) is not established, and the roles of the two
// accumulators (damage low/high) are inferred from how they are combined. The callees,
// powf and strstr are not defined here.
extern "C" {
int getDamageRangeAll__4itemPQ24item11descriptionPiT2i(const void* item, int* low, int* high, int flag);
int getArmorBonus__4itemPQ24item11description(const void* item);
float powf(float, float);
char* strstr(const char*, const char*);
}

struct GvEffect {
    short tag;
    short v[3];
};

struct GvDescription {
    const char* name;
    unsigned char gap0[20 - sizeof(void*)];
    int fixed;
    unsigned char gap1[29 - 24];
    signed char kind;
    signed char sub;
};

struct GvItem {
    const GvDescription* description;
    short count;
    unsigned char gap[36 - sizeof(void*) - sizeof(short)];
    GvEffect effects[4];
};

static_assert(sizeof(void*) != 4 || (__builtin_offsetof(GvItem, count) == 4 && __builtin_offsetof(GvItem, effects) == 36), "item layout");

static const float typeScaleTab[7] = {0x1.199998p+0f, 0x1.b33332p-1f, 0x1.b33332p-1f, 0x1.199998p+0f, 0x1p+0f, 0x1p+0f, 0x1p+0f};

static inline int find(const GvItem* item, int tag, int k) {
    for (int i = 0; i < 4; ++i) {
        if (item->effects[i].tag == tag) return item->effects[i].v[k];
    }
    return 0;
}

static inline float fl(int v) { return static_cast<float>(v); }

extern "C" int getValue__4itemPQ24item11description(const GvItem* item) {
    const GvDescription* d = item->description;
    if (d->fixed > 0) return item->count * d->fixed;
    float base = 0.0f;
    if (d->kind == 0 && d->sub != 5) {
        int low, high;
        getDamageRangeAll__4itemPQ24item11descriptionPiT2i(item, &low, &high, 0);
        float lo = fl(low) * 0x1.dd2d86p-2f;
        float hi = fl(high) * 0x1.dd2d86p-2f;
        for (int i = 0; i < 4; ++i) {
            int tag = item->effects[i].tag;
            switch (tag) {
            case 20:
                lo = fl(find(item, 20, 0)) * 0x1.70a3d6p-4f * lo + lo;
                hi = fl(find(item, 20, 0)) * 0x1.70a3d6p-4f * hi + hi;
                break;
            case 21: lo = lo + fl(find(item, 21, 0)) * 0x1.d1ed16p-2f; hi = hi + fl(find(item, 21, 1)) * 0x1.d1ed16p-2f; break;
            case 22: lo = lo + fl(find(item, 22, 0)) * 0x1.d3f854p-2f; hi = hi + fl(find(item, 22, 1)) * 0x1.d3f854p-2f; break;
            case 23: lo = lo + fl(find(item, 23, 0)) * 0x1.c6a7eep-2f; hi = hi + fl(find(item, 23, 1)) * 0x1.c6a7eep-2f; break;
            case 24: lo = lo + fl(find(item, 24, 0)) * 0x1.ced916p-2f; hi = hi + fl(find(item, 24, 1)) * 0x1.ced916p-2f; break;
            case 25: lo = lo + fl(find(item, 25, 0)) * 0x1.c28f5cp-3f; hi = hi + fl(find(item, 25, 1)) * 0x1.c28f5cp-3f; break;
            case 27:
                lo = lo + fl(find(item, 27, 0)) * 0x1.45a154p+0f * (lo * 0x1.47ae14p-7f);
                hi = hi + fl(find(item, 27, 1)) * 0x1.45a154p+0f * (hi * 0x1.47ae14p-7f);
                break;
            case 29:
                lo = lo + (fl(find(item, 29, 0)) * 0x1.47ae14p-7f + fl(find(item, 29, 0)) * 0x1.47ae14p-7f) * lo * 0x1.d6049ep-2f;
                hi = hi + (fl(find(item, 29, 1)) * 0x1.47ae14p-7f + fl(find(item, 29, 1)) * 0x1.47ae14p-7f) * hi * 0x1.d6049ep-2f;
                break;
            case 31: { float s = fl(find(item, 31, 0)) * 0x1.47ae14p-7f; lo = lo + fl(find(item, 31, 1)) * s * 0x1.e0c436p-1f; hi = hi + fl(find(item, 31, 2)) * s * 0x1.e0c436p-1f; break; }
            case 32: { float s = fl(find(item, 32, 0)) * 0x1.47ae14p-7f; lo = lo + fl(find(item, 32, 1)) * s * 0x1.e87228p-1f; hi = hi + fl(find(item, 32, 2)) * s * 0x1.e87228p-1f; break; }
            case 33: { float s = fl(find(item, 33, 0)) * 0x1.47ae14p-7f; lo = lo + fl(find(item, 33, 1)) * s * 0x1.2229c6p+7f; hi = hi + fl(find(item, 33, 1)) * s * 0x1.2229c6p+7f; break; }
            case 34: { float s = fl(find(item, 34, 0)) * 0x1.47ae14p-7f; lo = lo + fl(find(item, 34, 1)) * s * 0x1.04dcdap+0f; hi = hi + fl(find(item, 34, 2)) * s * 0x1.04dcdap+0f; break; }
            case 38: lo = lo + fl(find(item, 38, 0)) * 0x1.cfdf7ep-2f; hi = hi + fl(find(item, 38, 1)) * 0x1.cfdf7ep-2f; break;
            default: break;
            }
        }
        float scale = typeScaleTab[d->sub];
        hi = hi * scale;
        lo = lo * scale;
        base = (lo + hi) * 0x1p-1f - (hi - lo) * 0x1.999998p-4f;
    }
    if (d->kind == 1) base = fl(getArmorBonus__4itemPQ24item11description(item)) * 0x1.333332p-1f;
    else if (d->kind == 2) base = 0x1.4p+2f;
    float sum = powf(base, 0x1.f33332p+0f);
    for (int i = 0; i < 4; ++i) {
        int t = item->effects[i].tag;
        if (static_cast<unsigned>(t) >= 38u) continue;
        if (t == 13 && item->description->kind == 1) t = 26;
        switch (t) {
        case 0: sum = sum + powf(fl(find(item, 0, 0)), 0x1.da9df8p+0f); break;
        case 1: sum = sum + powf(fl(find(item, 1, 0)), 0x1.dffc32p+0f); break;
        case 2: sum = sum + powf(fl(find(item, 2, 0)), 0x1.abc546p+0f); break;
        case 3: sum = sum + powf(fl(find(item, 3, 0)), 0x1.81c92cp+0f); break;
        case 4: sum = sum + powf(fl(find(item, 4, 0)), 0x1.c830bp+0f); break;
        case 5: { float p = powf(fl(find(item, 5, 0)), 0x1.6459a6p+0f); sum = find(item, 5, 0) == 0 ? sum + p : (sum + p) + 0x1.08p+6f; break; }
        case 6: { float p = powf(fl(find(item, 6, 0)), 0x1.649b84p+0f); sum = find(item, 6, 0) == 0 ? sum + p : (sum + p) + 0x1.08p+6f; break; }
        case 7: { float p = powf(fl(find(item, 7, 0)), 0x1.e35286p+0f); sum = find(item, 7, 0) == 0 ? sum + p : (sum + p) + 0x1.08p+6f; break; }
        case 8: { float p = powf(fl(find(item, 8, 0)), 0x1.e3d60ep+0f); sum = find(item, 8, 0) == 0 ? sum + p : (sum + p) + 0x1.08p+6f; break; }
        case 9: { float p = powf(fl(find(item, 9, 0)), 0x1.f0e4b8p+0f); sum = find(item, 9, 0) == 0 ? sum + p : (sum + p) + 0x1.08p+6f; break; }
        case 10: { float p = powf(fl(find(item, 10, 0)), 0x1.eb84bap+0f); sum = find(item, 10, 0) == 0 ? sum + p : (sum + p) + 0x1.08p+6f; break; }
        case 11: { float p = powf(fl(find(item, 11, 0)), 0x1.ba9f16p+0f); sum = find(item, 11, 0) == 0 ? sum + p : (sum + p) + 0x1.a8p+5f; break; }
        case 12: { float p = powf(fl(find(item, 12, 0)), 0x1.b33268p+0f); sum = find(item, 12, 0) == 0 ? sum + p : (sum + p) + 0x1.a8p+5f; break; }
        case 13: sum = sum + powf(fl(find(item, 13, 0)), 0x1.820be4p-1f); break;
        case 26: { float p = powf(fl(find(item, 26, 0)), 0x1.ff3a9ap+0f); sum = find(item, 26, 0) == 0 ? sum + p : (sum + p) + 0x1.8p+5f; break; }
        case 28: { float p = powf(fl(find(item, 28, 0)), 0x1.137478p+1f); sum = find(item, 28, 0) == 0 ? sum + p : (sum + p) + 0x1.0cp+6f; break; }
        case 30: { float p = powf(fl(find(item, 30, 0)), 0x1.fba4c6p+0f); sum = find(item, 30, 0) == 0 ? sum + p : (sum + p) + 0x1.4p+6f; break; }
        case 37: sum = fl(find(item, 37, 0)) * 0x1.47ae14p-7f * sum + sum; break;
        default: break;
        }
    }
    sum = sum * 0x1.4p+5f;
    if (strstr(item->description->name, "spear") != nullptr) sum = sum * 0x1.47ae14p-8f;
    return static_cast<int>(sum + 0x1p-1f) * item->count;
}

// Observed lightCharacterShadow__FG6Point3P11LightResult at 0x0013d3c0, extent 828.
// (position, out). The Point3 arrives by pointer to a caller copy; the function
// copies it to a local and calls findLightsAtPoint__FG6Point3PP5Lighti(local, list, 32).
// Phase 1: for each light in the null-terminated list (index i), key =
//   (int)(|p - L.pos|^2 - L.radius^2) << 7 | i
// (L.pos is three floats at +48, L.radius a float at +28; squares and sums in x,y,z
// order). The three smallest signed keys are kept by insertion, all starting at
// 0x1000001f. Then list[31] is set to null, and the three chosen lights are
// list[key & 31] for the smallest, second and third key. They are written to a local
// array of three, to saveBestList[0..2], and saveValid is set to 1.
// Phase 2: for each chosen light up to the first null, d = L.pos - p, h = sqrt(dx*dx+dy*dy),
// r = sqrt((dx*dx+dy*dy)+dz*dz). If h > 63.5 then (dx,dy) are scaled by 127/h, else
// doubled. out[3i] and out[3i+1] receive the truncated dx,dy as bytes; out[3i+2] receives
// (int)(min(1, max(0, (L.radius - r) / L.radius)) * 126). Remaining triples get a zero in
// the third byte only. Finally out[9] = (int)(scene[0]*127), out[10] = (int)(scene[1]*127),
// out[11] = 24 (scene is the float block at 0x004f4060; saveValid and saveBestList are
// the image's own names for the data at 0x00483ab8 and 0x004fbf80).
// Inferred: the output is three (x, y, strength) shadow-caster records plus a light-
// direction pair; roles of the bytes and the constant 24 are not established.
// Unknown: LightResult's full size and meaning; the layout of Light beyond +28/+48.
// findLightsAtPoint, scene, saveBestList and saveValid are defined by other packages.
struct Point3 {
    float x, y, z;
};
struct Light {
    unsigned char pad0[28];
    float radius;
    unsigned char pad1[16];
    Point3 pos;
};
struct LightResult {
    signed char b[12];
};

extern "C" {
void findLightsAtPoint__FG6Point3PP5Lighti(Point3*, Light**, int);
extern float scene[];
extern Light* saveBestList[];
extern int saveValid;
}

namespace {
inline float min_s(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return b < a ? b : a;
#endif
}
inline float max_s(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return b > a ? b : a;
#endif
}
}  // namespace

extern "C" void lightCharacterShadow__FG6Point3P11LightResult(Point3* position, LightResult* out) {
    Point3 p = *position;
    Light* list[32];
    findLightsAtPoint__FG6Point3PP5Lighti(&p, list, 32);
    int k0 = 0x1000001f, k1 = 0x1000001f, k2 = 0x1000001f;
    if (list[0]) {
        int i = 0;
        Light* l;
        do {
            l = list[i];
            float dx = p.x - l->pos.x;
            float dy = p.y - l->pos.y;
            float dz = p.z - l->pos.z;
            float r2 = l->radius * l->radius;
            int key = (static_cast<int>(((dx * dx + dy * dy) + dz * dz) - r2) << 7) | i;
            ++i;
            int hi = key > k0 ? key : k0;
            k0 = key > k0 ? k0 : key;
            int hi2 = hi > k1 ? hi : k1;
            k1 = hi > k1 ? k1 : hi;
            k2 = hi2 > k2 ? k2 : hi2;
        } while (list[i]);
    }
    list[31] = 0;
    Light* best[3];
    best[0] = list[k0 & 31];
    best[1] = list[k1 & 31];
    best[2] = list[k2 & 31];
    saveBestList[0] = best[0];
    saveBestList[1] = best[1];
    saveBestList[2] = best[2];
    saveValid = 1;
    int i = 0;
    if (best[0]) {
        Light* l = best[0];
        for (;;) {
            float dx = l->pos.x - p.x;
            float dy = l->pos.y - p.y;
            float dz = l->pos.z - p.z;
            float xy2 = dx * dx + dy * dy;
            float h = __builtin_sqrtf(xy2);
            float r = __builtin_sqrtf(xy2 + dz * dz);
            if (63.5f < h) {
                float s = 127.0f / h;
                dx = dx * s;
                dy = dy * s;
            } else {
                dx = dx + dx;
                dy = dy + dy;
            }
            out->b[3 * i] = static_cast<signed char>(static_cast<int>(dx));
            out->b[3 * i + 1] = static_cast<signed char>(static_cast<int>(dy));
            out->b[3 * i + 2] = static_cast<signed char>(
                static_cast<int>(min_s(max_s((l->radius - r) / l->radius, 0.0f), 1.0f) * 126.0f));
            ++i;
            if (i >= 3) break;
            l = best[i];
            if (!l) break;
        }
    }
    for (; i < 3; ++i) out->b[3 * i + 2] = 0;
    out->b[9] = static_cast<signed char>(static_cast<int>(scene[0] * 127.0f));
    out->b[11] = 24;
    out->b[10] = static_cast<signed char>(static_cast<int>(scene[1] * 127.0f));
}

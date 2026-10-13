// Observed CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend at 0x010fb6d8, 568 bytes.
// With the world as this, a position and a result record `out`; the result is zero. Fields used
// (byte offsets): floats at this + 32 (cell), this + 36 / 40 / 44 (origin); words at this + 24 (n2),
// this + 28 (n3), this + 48 (n4); and the pointer at this + 84 to a byte table. The steps, in the
// float order of the original, are:
//   scale = (float)n4 / cell;  half = cell * 0.5;  inv = 1.0 / scale;  half2 = inv * 0.5;
//   X = (pos.x - origin.x) + half;  Z = (pos.z - origin.z) + half   (pos.y and origin.y are loaded and
//   subtracted but never reach the result);
//   xs = X - half2;  zs = Z - half2;  xi = (int)(xs * scale);  zi = (int)(zs * scale);
//   zi is clamped to [0, n2 * n4 - 1] and xi to [0, n3 * n4 - 1];
//   fz = clamp01(((zs - (float)zi * inv) * scale));  fx = clamp01(((xs - (float)xi * inv) * scale));
//   i0 = zi * (n3 * n4) + xi;  i1 = i0 + (xi != n3 * n4 - 1);
//   i2 = i0 + (n3 * n4) unless zi == n2 * n4 - 1 (then i0);  i3 = i1 + (n3 * n4) unless that same edge (then i1).
// The record receives: words (table byte at i0, i1, i2, i3) at out + 0, 4, 8, 12 and floats
// (1-fx)*(1-fz), fx*(1-fz), (1-fx)*fz, fx*fz at out + 16, 20, 24, 28.
// Inferred: this is bilinear sampling of a byte grid whose nodes are cell / n4 apart, the first node lying
// half a spacing in from an edge half a cell before the origin (host contract). Unknown: the
// meaning of the bytes (terrain profile ids by the callers' types) and of the words at this + 24 / 28 / 48.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
struct VIWorld;
struct VIVect3 { float x, y, z; };
struct VITerrainProfileBlend { int profile[4]; float weight[4]; };

static float clamp01(float v) {
    v = v > 0.0f ? v : 0.0f;
    return v < 1.0f ? v : 1.0f;
}

int CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend(VIWorld* world, const VIVect3* position,
                                                                 VITerrainProfileBlend* out) {
    const char* w = reinterpret_cast<const char*>(world);
    const float* origin = reinterpret_cast<const float*>(w + 36);
    int n4 = *reinterpret_cast<const int*>(w + 48);
    float cell = *reinterpret_cast<const float*>(w + 32);
    int n3 = *reinterpret_cast<const int*>(w + 28);
    int n2 = *reinterpret_cast<const int*>(w + 24);
    float scale = static_cast<float>(n4) / cell;
    float half = cell * 0.5f;
    int zlimit = n2 * n4 - 1;
    int xlimit = n3 * n4 - 1;
    float inv = 1.0f / scale;
    float half2 = inv * 0.5f;
    float x = (position->x - origin[0]) + half;
    float z = (position->z - origin[2]) + half;
    float xs = x - half2;
    float zs = z - half2;
    int zi = static_cast<int>(zs * scale);
    int xi = static_cast<int>(xs * scale);
    if (zi < 0) zi = 0;
    if (zi > zlimit) zi = zlimit;
    if (xi < 0) xi = 0;
    if (xi > xlimit) xi = xlimit;
    float fz = clamp01((zs - static_cast<float>(zi) * inv) * scale);
    float fx = clamp01((xs - static_cast<float>(xi) * inv) * scale);
    int stride = n3 * n4;
    int i0 = zi * stride + xi;
    int i1 = i0 + (xi != xlimit ? 1 : 0);
    int i2 = zi == zlimit ? i0 : i0 + stride;
    int i3 = zi == zlimit ? i1 : i1 + stride;
    const unsigned char* table = reinterpret_cast<const unsigned char*>(NORRATH_TRANSLATE(*reinterpret_cast<const unsigned*>(w + 84)));
    out->profile[0] = table[i0];
    out->profile[1] = table[i1];
    out->profile[2] = table[i2];
    out->profile[3] = table[i3];
    out->weight[0] = (1.0f - fx) * (1.0f - fz);
    out->weight[1] = fx * (1.0f - fz);
    out->weight[2] = (1.0f - fx) * fz;
    out->weight[3] = fx * fz;
    return 0;
}
}

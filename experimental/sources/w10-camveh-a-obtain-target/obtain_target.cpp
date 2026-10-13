// Observed obtainTarget__6CameraP6Point3 at 0x0026ee98, 1672 bytes. Arguments:
// the camera and a Point3 `out` that is written. Returns int: 0 in the one case
// below marked, otherwise 1. Nothing here was executed on the EE. Field
// meanings are not established, so offsets stay numeric. W/F/B are the word,
// float and signed byte at that camera offset; PL(p) is the 23984-byte entry p
// of `player`, and T(p) the pointer at PL(p) + 21872 (a position holder with a
// Point3 at +16).
//
// 1. found = the first i in 0..nmPlayers with netPlayerIsLocal__Fi(i) nonzero,
//    the float at PL(i)+172 not equal to 0.0 and T(i) not null; found = nmPlayers
//    when there is none.
// 2. When localNmPlayersAlive == 0: return 0 if netNetworkMode == 0. If
//    nmPlayersAlive == 0 as well and gameDeathTimerSingle[B(353)] (halfwords,
//    0x00484f88) is zero, return 1 with nothing written. Otherwise with w =
//    B(353), return 1 with nothing written if T(w) is null; else
//    hudDisplayNetCamToggle = 1 and out = the Point3 at T(w)+16 with F(252) added
//    to z.
// 3. Otherwise B(353) = (byte)found, then:
//    a. W(100) != 0: t = T(playersAlive[0]) (the int array at 0x0042a3e8) and
//       v = the pointer at t + 1328; out = Point3 at v+16, plus -(m1, m4, m7)
//       * (F(140) * 0.125) where m1, m4, m7 are floats 1, 4 and 7 of
//       worldCameraMatrix. F(252) is not added on this path.
//    b. else when nmPlayers != 0 and (W(72) == 0 or MC_SavingOrLoading != 0):
//       each of the n = localNmPlayersAlive listed players p
//       (localPlayersAlive[]) has the Point3 at T(p)+16 sent through
//       matTransform__FP6Point3P8Matrix34T0(&P[i], worldCameraMatrix, ...); then
//       P[i].x and P[i].y are divided by P[i].z. minz and maxz are the least
//       and greatest z (start +100000.0 and -100000.0). The indices of the least
//       x, least y, greatest x and greatest y (strict compares, the earliest
//       wins) are picked. mid = (maxz + minz) * 0.5; the Point3 c = ((maxx.x +
//       minx.x) * 0.5 * mid, (maxy.y + miny.y) * 0.5 * mid, mid). With
//       |.| through fptodp, dpcmp, dpsub and dptofp: F(204) = min(1.0, |minx.x -
//       maxx.x|), F(208) = min(1.0, |miny.y - maxy.y|), F(212) = min(1.0,
//       sqrt(F(204)^2 + F(208)^2)). Then out = matUntransform34Vec__FP6Point3P8
//       Matrix34T0(c, worldCameraMatrix) plus worldCameraPos, and F(252) added
//       to z.
//    c. else out is the average of the Point3s at T(p)+16 over the listed
//       players, with F(252) added to z.
//    Returns 1.
typedef unsigned long long Double;

extern "C" {
Double fptodp(float value);
int dpcmp(Double a, Double b);
Double dpsub(Double a, Double b);
float dptofp(Double a);
int netPlayerIsLocal__Fi(int index);
void* matTransform__FP6Point3P8Matrix34T0(float* out, float* matrix, float* in);
void* matUntransform34Vec__FP6Point3P8Matrix34T0(float* out, float* matrix, float* in);
extern int nmPlayers;
extern int nmPlayersAlive;
extern int localNmPlayersAlive;
extern int localPlayersAlive[];
extern int playersAlive[];
extern signed char netNetworkMode;
extern signed char MC_SavingOrLoading;
extern signed char hudDisplayNetCamToggle;
extern short gameDeathTimerSingle[];
extern float worldCameraMatrix[];
extern float worldCameraPos[];
extern unsigned char player[];
}

typedef int __attribute__((may_alias)) AliasedInt;
typedef float __attribute__((may_alias)) AliasedFloat;
typedef unsigned char* __attribute__((may_alias)) AliasedPtr;

#define WORD(off) (*reinterpret_cast<AliasedInt*>(camera + (off)))
#define FLOAT(off) (*reinterpret_cast<AliasedFloat*>(camera + (off)))
#define BYTE(off) (*reinterpret_cast<signed char*>(camera + (off)))

static inline float fp_min(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a < b ? a : b;
#endif
}

static inline float sqrt_s(float value) {
#if defined(__mips__)
    float out;
    __asm__("sqrt.s %0, %1" : "=f"(out) : "f"(value));
    return out;
#else
    return __builtin_sqrtf(value);
#endif
}

static inline unsigned char* holder(int p) {
    return *reinterpret_cast<AliasedPtr*>(player + 23984 * p + 21872);
}

static inline float absDiff(float a, float b) {
    Double d = fptodp(a - b);
    if (dpcmp(d, 0) < 0) d = dpsub(0, d);
    return dptofp(d);
}

extern "C" int obtainTarget__6CameraP6Point3(unsigned char* camera, float* out) {
    int found = 0;
    for (; found < nmPlayers; ++found) {
        if (netPlayerIsLocal__Fi(found) == 0) continue;
        if (*reinterpret_cast<AliasedFloat*>(player + 23984 * found + 172) == 0.0f) continue;
        if (holder(found) == nullptr) continue;
        break;
    }

    if (localNmPlayersAlive == 0) {
        if (netNetworkMode == 0) return 0;
        if (nmPlayersAlive == 0 && gameDeathTimerSingle[BYTE(353)] == 0) return 1;
        const unsigned char* t = holder(BYTE(353));
        if (t == nullptr) return 1;
        hudDisplayNetCamToggle = 1;
        out[0] = *reinterpret_cast<const AliasedFloat*>(t + 16);
        out[1] = *reinterpret_cast<const AliasedFloat*>(t + 20);
        out[2] = *reinterpret_cast<const AliasedFloat*>(t + 24) + FLOAT(252);
        return 1;
    }

    BYTE(353) = static_cast<signed char>(found);
    if (WORD(100) != 0) {
        const unsigned char* t = holder(playersAlive[0]);
        const unsigned char* v = *reinterpret_cast<AliasedPtr*>(const_cast<unsigned char*>(t) + 1328);
        const float x = *reinterpret_cast<const AliasedFloat*>(v + 16);
        const float y = *reinterpret_cast<const AliasedFloat*>(v + 20);
        const float z = *reinterpret_cast<const AliasedFloat*>(v + 24);
        out[0] = x;
        out[1] = y;
        out[2] = z;
        const float scale = FLOAT(140) * 0.125f;
        const float dz = -worldCameraMatrix[7] * scale;
        const float dx = -worldCameraMatrix[1] * scale;
        const float dy = -worldCameraMatrix[4] * scale;
        out[2] = z + dz;
        out[0] = x + dx;
        out[1] = y + dy;
        return 1;
    }

    if (nmPlayers != 0 && (WORD(72) == 0 || MC_SavingOrLoading != 0)) {
        const int n = localNmPlayersAlive;
        float p[4][3];
        float minz = 100000.0f;
        float maxz = -100000.0f;
        for (int i = 0; i < n; ++i) {
            const unsigned char* t = holder(localPlayersAlive[i]);
            matTransform__FP6Point3P8Matrix34T0(p[i], worldCameraMatrix, reinterpret_cast<float*>(const_cast<unsigned char*>(t) + 16));
            const float z = p[i][2];
            p[i][0] = p[i][0] / z;
            p[i][1] = p[i][1] / z;
            if (z < minz) minz = z;
            if (maxz < z) maxz = z;
        }
        int minx = 0, miny = 0, maxx = 0, maxy = 0;
        for (int i = 1; i < n; ++i) {
            if (p[i][0] < p[minx][0]) minx = i;
            if (p[i][1] < p[miny][1]) miny = i;
            if (p[maxx][0] < p[i][0]) maxx = i;
            if (p[maxy][1] < p[i][1]) maxy = i;
        }
        const float mid = (maxz + minz) * 0.5f;
        float c[3];
        c[2] = mid;
        c[0] = (p[maxx][0] + p[minx][0]) * 0.5f * mid;
        c[1] = (p[maxy][1] + p[miny][1]) * 0.5f * mid;
        const float w = fp_min(1.0f, absDiff(p[minx][0], p[maxx][0]));
        FLOAT(204) = w;
        const float h = fp_min(1.0f, absDiff(p[miny][1], p[maxy][1]));
        FLOAT(208) = h;
        FLOAT(212) = fp_min(1.0f, sqrt_s(w * w + h * h));
        matUntransform34Vec__FP6Point3P8Matrix34T0(out, worldCameraMatrix, c);
        out[0] = out[0] + worldCameraPos[0];
        out[1] = out[1] + worldCameraPos[1];
        out[2] = out[2] + worldCameraPos[2];
    } else {
        out[0] = 0.0f;
        out[2] = 0.0f;
        out[1] = 0.0f;
        const int n = localNmPlayersAlive;
        for (int i = 0; i < n; ++i) {
            const unsigned char* t = holder(localPlayersAlive[i]);
            out[0] = out[0] + *reinterpret_cast<const AliasedFloat*>(t + 16);
            out[1] = out[1] + *reinterpret_cast<const AliasedFloat*>(t + 20);
            out[2] = out[2] + *reinterpret_cast<const AliasedFloat*>(t + 24);
        }
        const float count = static_cast<float>(localNmPlayersAlive);
        out[2] = out[2] / count;
        out[0] = out[0] / count;
        out[1] = out[1] / count;
    }
    out[2] = out[2] + FLOAT(252);
    return 1;
}

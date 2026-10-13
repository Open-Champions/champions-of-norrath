// Observed determineBestController__6Camera at 0x0026f528, 628 bytes. Returns
// a pad index as an int. Nothing here was executed on the EE.
//
// n is localNmPlayersAlive (0x00484f48) and list is the int array
// localPlayersAlive (0x0042a3d8). Pad entry k is the 88-byte record k of
// `pad` (0x0034beb8); X(k) is its float at +28, Y(k) its float at +32, and
// FLAGS(k) its word at +12. Absolute values are taken in doubles through the
// original's soft routines fptodp, dpcmp (is the first argument below zero)
// and dpsub (0 - a), compared with dpcmp, and added with dpadd.
// Let cur be the word at camera+68.
// First pass over list[0..n): when list[i] == cur, cur is returned if
// |X(cur)| >= 0.5, or if |Y(cur)| >= 0.9f (the double 0x3fecccccc0000000), or
// if FLAGS(cur) has bit 0x400. Otherwise the scan goes on.
// Second pass, which also runs when n <= 0: the result starts as list[0]
// (read even if n <= 0) and the best score as 0.0. For each list[i] = k in
// order, score = dptofp(dpadd(|X(k)|, |Y(k)|)), plus 0.5 when FLAGS(k) has bit
// 0x400. When the best score is below this score (strict), k becomes the result and the
// score the new best. The result is returned.
// What the pad fields and the list mean is not established here.
typedef unsigned long long Double;

extern "C" {
Double fptodp(float value);
int dpcmp(Double a, Double b);
Double dpsub(Double a, Double b);
Double dpadd(Double a, Double b);
float dptofp(Double a);
extern unsigned char pad[];
extern int localPlayersAlive[];
extern int localNmPlayersAlive;
}

typedef int __attribute__((may_alias)) AliasedInt;
typedef float __attribute__((may_alias)) AliasedFloat;

static inline Double padAbs(int k, int offset) {
    Double v = fptodp(*reinterpret_cast<AliasedFloat*>(pad + 88 * k + offset));
    if (dpcmp(v, 0) < 0) v = dpsub(0, v);
    return v;
}

extern "C" int determineBestController__6Camera(unsigned char* camera) {
    const int n = localNmPlayersAlive;
    if (n > 0) {
        const int cur = *reinterpret_cast<AliasedInt*>(camera + 68);
        for (int i = 0; i < n; ++i) {
            if (localPlayersAlive[i] == cur) {
                if (dpcmp(padAbs(cur, 28), 0x3fe0000000000000ULL) >= 0) return cur;
                if (dpcmp(padAbs(cur, 32), 0x3fecccccc0000000ULL) >= 0) return cur;
                if (*reinterpret_cast<AliasedInt*>(pad + 88 * cur + 12) & 0x400) return cur;
            }
        }
    }
    int result = localPlayersAlive[0];
    float best = 0.0f;
    for (int i = 0; i < n; ++i) {
        const int k = localPlayersAlive[i];
        const Double x = padAbs(k, 28);
        const Double y = padAbs(k, 32);
        float score = dptofp(dpadd(x, y));
        if (*reinterpret_cast<AliasedInt*>(pad + 88 * k + 12) & 0x400) score += 0.5f;
        if (best < score) {
            result = k;
            best = score;
        }
    }
    return result;
}

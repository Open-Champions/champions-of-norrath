// Host contract for fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii; not an EE input. The three objects and the
// position are left untouched. The authored function was also compared with the original in a private R5900 run (6 scenarios).
#include "fx_run.cpp"
int main() {
    unsigned char a[64], b[64], c[64];
    for (int i = 0; i < 64; ++i) { a[i] = static_cast<unsigned char>(i + 1); b[i] = static_cast<unsigned char>(i * 2 + 1); c[i] = static_cast<unsigned char>(i * 3 + 1); }
    Point3 p = {1.0f, 2.0f, 3.0f};
    fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii(a, b, c, p, 4.0f, 5, 6);
    for (int i = 0; i < 64; ++i) if (a[i] != static_cast<unsigned char>(i + 1) || b[i] != static_cast<unsigned char>(i * 2 + 1) || c[i] != static_cast<unsigned char>(i * 3 + 1)) return 1;
    if (p.x != 1.0f || p.y != 2.0f || p.z != 3.0f) return 2;
    return 0;
}

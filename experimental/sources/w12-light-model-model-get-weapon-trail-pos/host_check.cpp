// Host contract for modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1. Not an EE plan input.
#include <cstdio>
#include <initializer_list>
#include <cstring>
struct Point3 { float x, y, z; };
extern "C" int modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1(const unsigned char*, Point3*, Point3*);
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char model[4200];
    std::memset(model, 0, sizeof model);
    Point3 a{9, 9, 9}, b{9, 9, 9};
    int r = modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1(model, &a, &b);
    expect(r == 0, "default result");
    expect(a.x == 0 && a.y == 4.5f && a.z == 0, "default base");
    expect(b.x == 0 && b.y == 38.0f && b.z == 0, "default tip");
    for (int k : {3, 4, 17, 255}) {
        std::memset(model, 0, sizeof model);
        float vals[6] = {1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f};
        std::memcpy(model + k * 16, vals, 12);
        std::memcpy(model + k * 16 + 12, vals + 3, 12);
        model[39] = static_cast<unsigned char>(k);
        a = Point3{9, 9, 9}; b = Point3{9, 9, 9};
        r = modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1(model, &a, &b);
        expect(r == 1, "record result");
        expect(a.x == 1.5f && a.y == 2.5f && a.z == 3.5f, "record base");
        expect(b.x == 4.5f && b.y == 5.5f && b.z == 6.5f, "record tip");
    }
    if (failures) return 1;
    std::printf("modelGetWeaponTrailPos contract passed\n");
    return 0;
}

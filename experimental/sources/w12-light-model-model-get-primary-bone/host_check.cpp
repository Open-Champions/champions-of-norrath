// Host contract for modelGetPrimaryBone__FP12_modelHeaderi. Not an EE plan input.
#include <cstdio>
#include <initializer_list>
#include <cstring>
extern "C" int modelGetPrimaryBone__FP12_modelHeaderi(const unsigned char*, int);
int main() {
    static unsigned char buf[8192];
    int failures = 0;
    struct Case { signed char a, b; int index; };
    const Case cases[] = {{0, 0, 0}, {0, 0, 3}, {1, 1, 0}, {2, 3, 4}, {10, 2, 2}, {-1, 5, 1}, {3, -2, 6}, {0, 1, 1}};
    for (const Case& c : cases) {
        std::memset(buf, 0, sizeof buf);
        buf[18] = static_cast<unsigned char>(c.a);
        buf[38] = static_cast<unsigned char>(c.b);
        const unsigned char* model = buf;
        int base = c.b * (c.a * 4 + 4) + c.index * 10 + 46;
        if (base < 0 || base >= 8192 || base == 18 || base == 38) continue;
        for (int v : {0, 1, 127, -1, -128}) {
            buf[base] = static_cast<unsigned char>(v);
            int got = modelGetPrimaryBone__FP12_modelHeaderi(model, c.index);
            if (got != v) { std::fprintf(stderr, "fail: a=%d b=%d i=%d v=%d got %d\n", c.a, c.b, c.index, v, got); ++failures; }
        }
    }
    if (failures) return 1;
    std::printf("modelGetPrimaryBone contract passed\n");
    return 0;
}

// Host contract for modelGetCollision__FP12_modelHeaderi. Not an EE plan input.
#include <cstdio>
#include <initializer_list>
extern "C" unsigned char* modelGetCollision__FP12_modelHeaderi(unsigned char*, int);
int main() {
    static unsigned char buf[4096];
    int failures = 0;
    buf[37] = 0;
    if (modelGetCollision__FP12_modelHeaderi(buf, 3) != nullptr) { std::fprintf(stderr, "fail: zero count\n"); ++failures; }
    for (int count : {1, 2, 7, 255}) {
        buf[37] = static_cast<unsigned char>(count);
        for (int i : {0, 1, 5, -1}) {
            unsigned char* want = buf + count * 16 + i * 24;
            if (modelGetCollision__FP12_modelHeaderi(buf, i) != want) { std::fprintf(stderr, "fail: count %d index %d\n", count, i); ++failures; }
        }
    }
    if (failures) return 1;
    std::printf("modelGetCollision contract passed\n");
    return 0;
}

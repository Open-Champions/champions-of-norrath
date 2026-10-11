// Host contract for arListBack. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char* arListBack;
int main() {
    if (arListBack != nullptr) {
        std::fprintf(stderr, "fail: arListBack\n");
        return 1;
    }
    arListBack = reinterpret_cast<unsigned char*>(1);
    if (arListBack == nullptr) return 1;
    return 0;
}

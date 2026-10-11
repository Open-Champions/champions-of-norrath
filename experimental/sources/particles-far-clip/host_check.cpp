// Host contract for farClip. Not an EE plan input.
#include <cstdio>
extern "C" float farClip;
int main() {
    if (sizeof farClip != 4u) return 1;
    if (farClip != 0.f) {
        std::fprintf(stderr, "fail: farClip\n");
        return 1;
    }
    return 0;
}

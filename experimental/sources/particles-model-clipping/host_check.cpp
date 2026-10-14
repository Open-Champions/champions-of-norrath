// Host contract for modelClippingOn. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char modelClippingOn;
int main() {
    if (sizeof modelClippingOn != 1u) return 1;
    if (modelClippingOn != 0) {
        std::fprintf(stderr, "fail: modelClippingOn\n");
        return 1;
    }
    return 0;
}

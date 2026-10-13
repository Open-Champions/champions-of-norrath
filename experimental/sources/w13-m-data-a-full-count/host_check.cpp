// Host contract for fullCount. Not an EE plan input.
#include <cstdio>
extern "C" int fullCount;
int main() {
    if (sizeof fullCount != 4u) return 1;
    if (fullCount != 0) {
        std::fprintf(stderr, "fail: fullCount\n");
        return 1;
    }
    return 0;
}

// Host contract for trailSequenceNm. Not an EE plan input.
#include <cstdio>
extern "C" unsigned trailSequenceNm;
int main() {
    if (sizeof trailSequenceNm != 4u) return 1;
    if (trailSequenceNm != 1u) {
        std::fprintf(stderr, "fail: trailSequenceNm\n");
        return 1;
    }
    return 0;
}

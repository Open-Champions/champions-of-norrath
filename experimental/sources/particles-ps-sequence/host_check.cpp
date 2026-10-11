// Host contract for psSequenceNm. Not an EE plan input.
#include <cstdio>
extern "C" unsigned psSequenceNm;
int main() {
    if (sizeof psSequenceNm != 4u) return 1;
    if (psSequenceNm != 1u) {
        std::fprintf(stderr, "fail: psSequenceNm\n");
        return 1;
    }
    return 0;
}

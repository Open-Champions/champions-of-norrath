// Host contract for P_FrameNum. Not an EE plan input.
#include <cstdio>
extern "C" unsigned short P_FrameNum;
int main() {
    if (sizeof P_FrameNum != 2u) return 1;
    if (P_FrameNum != 0) {
        std::fprintf(stderr, "fail: P_FrameNum\n");
        return 1;
    }
    return 0;
}

// Host contract for __EH_FRAME_BEGIN__. Not an EE plan input. The marker has no storage; the
// check is that it is a zero-sized, four-byte aligned array that can be named by the callers.
#include <cstdio>
#include "eh_frame_begin.cpp"
extern "C" char __EH_FRAME_BEGIN__[];
int main() {
    if (sizeof(__EH_FRAME_BEGIN__) != 0 && sizeof(__EH_FRAME_BEGIN__) != 1) return 1;   // zero-sized extension
    if ((reinterpret_cast<__UINTPTR_TYPE__>(__EH_FRAME_BEGIN__) & 3) != 0) return 2;
    std::puts("__EH_FRAME_BEGIN__ ok");
    return 0;
}
